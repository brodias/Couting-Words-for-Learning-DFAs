import gurobipy as gp
from gurobipy import GRB

from random import *
import time

# Functions on the positive words
def computePrefix(l):
	L = []
	for i in range(len(l)+1):
		L.append([ l[j] for j in range(i) ])
	return L

def simplify(A):
	t = [[]]
	for l in A:
		if l not in t:  
			t.append(l)
	return t

def allPrefixes(L):
	t = [[]]
	for l in L:
		# ~ t = set(t + computePrefix(l))
		for x in computePrefix(l):  
			if x not in t:  
				t.append(x)
	return t

def generate_words(k,length,a):
	L = []
	for i in range(k):
		x = randrange(length+1)
		l = []
		for i in range(x):
			l.append(randrange(0,a+1))
		L.append(l)
	return L

def get_ind(L,l):
	for i in range(len(L)):
		if L[i] == l:
			return i
	return -1

# ~ n : number of states
# ~ Alph : the alphabet (described as a list)
# ~ m : the model
def transition_variable(n,Alph,m): 
	# ~ Variable d_{q,a,q'} = T iff \delta(q,a) = q'
	D = [ [ [ m.addVar(vtype=GRB.BINARY, name="d_q%s_q%s {R %s}" % (i, j, a)) 
				for j in range(n) ] 
				for a in Alph ] 
				for i in range(n)]

	# Unique transition
	for i in range(n):
		for a in Alph: 
			m.addConstr(sum([ D[i][a][j] for j in range(n)]) == 1, "unique transition")
	
	for a in Alph: 
		m.addConstr(D[1][a][0] == 0, "unique transition")
	m.addConstr(D[0][2][1] == 1, "unique transition")
	
	return D

# ~ n : number of states
# ~ Alph : the alphabet (described as a list)
# ~ RealPref : the set of prefixes of the positive words
# ~ D : the transition variables
# ~ m : the model
def execution_variable(n,Alph,Pref,D,m): 
	# Variable x_l_i = T iff \delta*(q_0,l) = i
	X = [ [ #m.addVar(vtype=GRB.BINARY, name="Word to State: %s -> %s" % (l, i)) 
			m.addVar(vtype=GRB.BINARY, name="Word to State") 
			for i in range(n) ] 
			for l in Pref ]

	m.addConstr(X[get_ind(Pref,[])][0] == 1, "empty_word_initial_state")

	for l in range(len(Pref)):
		m.addConstr(sum([ X[l][i] for i in range(n)]) == 1, "unique_destination_state")
	
	for i1 in range(n):
		for i2 in range(n):
			for l1 in range(len(Pref)):
				for l2 in range(len(Pref)):
					for a in Alph:
						if Pref[l2] == Pref[l1] + [a]:
							m.addConstr(X[l1][i1] + D[i1][a][i2] <= 1 + X[l2][i2], "transition")
	
	return X

# ~ n : number of states
# ~ Alph : the alphabet (described as a list)
# ~ RealPref : the set of prefixes of the positive words
# ~ P : the set of positive words
# ~ X : the execution variables
# ~ m : the model
def final_variable(n,Alph,Pref,P,X,m): 
	# Final states
	F = [ m.addVar(vtype=GRB.BINARY, name="f_%s" % (i)) for i in range(n) ]
	for i in range(n):
		for x in range(len(Pref)):
			if Pref[x] in P:
				m.addConstr(X[x][i] <= F[i], "accept_words")
	
	return F

# ~ n : number of states
# ~ l : size
# ~ Alph : the alphabet (described as a list)
# ~ D : the transition variables
# ~ F : the final variables
# ~ m : the model
def counting_variable_linear_additional_constraint(n,l,Alph,D,F,m): 
	M = (len(Alph))**l+1
	
	# Counting accepted words			
	N = [ [ m.addVar(vtype=GRB.INTEGER, name="Number_q%s (Step %s)" % (i,j)) 
			for j in range(l) ] 
			for i in range(n) ]

	m.addConstr(N[0][0] == 1, "initialization")
	for i in range(1,n):
		m.addConstr(N[i][0] == 0, "initialization")

	# Intermediate variables 
	I = [ [ [ [ m.addVar(vtype=GRB.INTEGER, name="i_%s_%s_%s_%s" % (i1,a,i2,j)) 
			for j in range(l-1) ] 
			for i2 in range(n) ]
			for a in Alph ]
			for i1 in range(n) ]

	# Add constraint : "I[i1][a][i2][j] = N[i1][j] * D[i1][a][i2]"
	for i1 in range(n):
		for a in Alph:
			for i2 in range(n):
				for j in range(l-1):
					m.addConstr(N[i1][j] + D[i1][a][i2] * M <= I[i1][a][i2][j] + M, "intermediate_step")
					m.addConstr(I[i1][a][i2][j] <= D[i1][a][i2] * M, "intermediate_step")
					m.addConstr(0 <= I[i1][a][i2][j], "intermediate_step")
			
	for i2 in range(n):	
		for j in range(1,l):
				m.addConstr(N[i2][j] == sum([I[i1][a][i2][j-1] for i1 in range(n) for a in Alph]),"update")

	# Additional constraints
	for j in range(1,l):
		m.addConstr(sum([N[i][j] for i in range(n)]) == (len(Alph))**j,"Exact_bound_update")
		for i in range(n):	
			m.addConstr(N[i][j] <= (len(Alph))**j,"Bound_update")

	# Intermediate variables Final counting
	C = [ [ m.addVar(vtype=GRB.INTEGER, name="C_q%s (Step %s)" % (i,j)) 
			for j in range(l) ] 
			for i in range(n) ]

	# Add constraint : "C[i][j] = N[i][j] * F[i]"
	for i in range(n):
		for j in range(l):
			m.addConstr(N[i][j] + F[i] * M <= C[i][j] + M, "intermediate_step_final_counting")
			m.addConstr(C[i][j] <= F[i] * M, "intermediate_step")
			m.addConstr(0 <= C[i][j], "intermediate_step")

	return C	

def read_file(nameFile):
	with open(nameFile) as f: s = f.read()
	L = []
	i = 0
	while (i < len(s)) and (s[i] != '-'):
		l = []
		while (i < len(s)) and (s[i] != '\n'):
			if (s[i] == 'a'):
				l += [0]
			elif (s[i] == 'b'):
				l += [1]
			elif (s[i] == 'c'):
				l += [2]
			i += 1
		if (l != []):
			L.append(l)
		i += 1
	return L

# ~ n : number of states
# ~ Alph : the alphabet (described as a list)
# ~ D : the transition variables
# ~ F : the final variables
# ~ m : the model
def counting_number_of_states(n,Alph,D,F,m):	
	# ~ Variable r_{i,k} = T iff there is a word (of size at most k) from 0 to i
	Rc = [ [ m.addVar(vtype=GRB.BINARY, name="r_%s_%s" % (i,k)) 
				for k in range(n) ]
				for i in range(n) ]
	m.addConstr(Rc[0][0] == 1, "initialization")
	for i in range(1,n):
		m.addConstr(Rc[i][0] == 1, "initialization")
	
	IRc = [ [ [ [ m.addVar(vtype=GRB.BINARY, name="Ensure_r_%s_%s_%s_%s" % (i,a,i1,k)) 
							for k in range(n-1) ]
							for i1 in range(0,n) ]
							for a in Alph ]
							for i in range(0,n)]
	
	# EnsureReach[i1][a][i][k] = Reach[i1][k] * D[i1][a][i]
	for i in range(0,n):
		for a in Alph:
			for i1 in range(0,n):
				for k in range(n-1):
					m.addConstr(Rc[i1][k] + D[i1][a][i] <= 1 + IRc[i1][a][i][k], "relevant states")
					m.addConstr(IRc[i1][a][i][k] <= D[i1][a][i], "relevant states")
					m.addConstr(IRc[i1][a][i][k] <= Rc[i1][k], "relevant states")
	
	# Reachable states
	for i in range(1,n):
		for k in range(1,n):
			m.addConstr(Rc[i][k] <= sum([IRc[i1][a][i][k-1] for a in Alph for i1 in range(0,n)]), "reachable states")
			for a in Alph: 
				for i1 in range(0,n):
					m.addConstr(IRc[i1][a][i][k-1] <= Rc[i][k], "reachable states")
				
	# ~ Variable cr_{i,k} = T iff there is a word (of size at most k) from i to an accepting state
	CRc = [ [ m.addVar(vtype=GRB.BINARY, name="cr_%s_%s" % (i,k)) 
				for k in range(n) ]
				for i in range(n) ]
	
	for i in range(n):
		m.addConstr(CRc[i][0] == F[i], "initialization")
	
	ICRc = [ [ [ [ m.addVar(vtype=GRB.BINARY, name="Ensure_cr_%s_%s_%s_%s" % (i,a,i1,k)) 
								for k in range(n-1) ]
								for i1 in range(0,n) ]
								for a in Alph ]
								for i in range(0,n)]
	
	# EnsureCoReach[i1][a][i][k] = CoReach[i][k] * D[i1][a][i]
	for i in range(0,n):
		for a in Alph:
			for i1 in range(0,n):
				for k in range(n-1):
					m.addConstr(CRc[i1][k] + D[i][a][i1] <= 1 + ICRc[i][a][i1][k], "relevant states")
					m.addConstr(ICRc[i][a][i1][k] <= D[i][a][i1], "relevant states")
					m.addConstr(ICRc[i][a][i1][k] <= CRc[i1][k], "relevant states")
	
	# CoReachable states
	for i in range(1,n):
		for k in range(1,n):
			m.addConstr(CRc[i][k] <= sum([ICRc[i][a][i1][k-1] for a in Alph for i1 in range(0,n)]), "reachable states")
			for a in Alph: 
				for i1 in range(0,n):
					m.addConstr(ICRc[i][a][i1][k-1] <= Rc[i][k], "reachable states")
					
	# ~ Variable rcr_{i} = T iff i is reach and co_reach
	RcCRc = [ m.addVar(vtype=GRB.BINARY, name="rcr_%s" % (i)) for i in range(n)]
	for i in range(0,n):
		m.addConstr(Rc[i][n-1] + CRc[i][n-1] <= 1 + RcCRc[i],"co_reach")
		m.addConstr(RcCRc[i] <= Rc[i][n-1],"co_reach")
		m.addConstr(RcCRc[i] <= CRc[i][n-1],"co_reach")
	
	# ~ Variable rncr_{i} = T iff i is reach and not co_reach
	RcNCRc = [ m.addVar(vtype=GRB.BINARY, name="rncr_%s" % (i)) for i in range(n)]
	for i in range(0,n):
		m.addConstr(Rc[i][n-1] + (1- CRc[i][n-1]) <= 1 + RcNCRc[i],"co_reach")
		m.addConstr(RcNCRc[i] <= Rc[i][n-1],"co_reach")
		m.addConstr(RcNCRc[i] <= 1 - CRc[i][n-1],"co_reach")
	
	Sink = m.addVar(vtype=GRB.BINARY, name="exists_a_sink_state")
	m.addConstr(sum([RcNCRc[i] for i in range(n)]) <= Sink * n,"co_reach")
	m.addConstr(Sink <= sum([RcNCRc[i] for i in range(n)]),"co_reach")
	
	Nb_st = m.addVar(vtype=GRB.INTEGER, name="number_of_states")
	m.addConstr(Nb_st == Sink + sum([RcCRc[i] for i in range(n)]),"co_reach")
	
	return Nb_st

L = [	'Traces/OrignalTraces/f:01-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:02-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:03-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:04-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:05-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:06-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:07-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:08-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:09-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:10-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:11-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:12-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:13-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:14-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:15-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:16-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:17-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:18-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:19-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:20-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:21-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:22-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:23-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:24-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:25-nw:1000-ml:02-0.words',
		'Traces/OrignalTraces/f:26-nw:1000-ml:03-0.words',
		'Traces/OrignalTraces/f:27-nw:1000-ml:04-0.words',
		'Traces/OrignalTraces/f:28-nw:1000-ml:05-0.words']

# ~ n : number of states
def learn_language_minimal_automaton(n,nameFile):
	Alph = [x for x in range(3)]
	M = (len(Alph))**(2*n)+1

	P = read_file(nameFile)
	Pref = allPrefixes(P)
	# ~ print(P)
	
	m = gp.Model("Test")
	# ~ m.setParam('TimeLimit', 30)
	D = transition_variable(n,Alph,m)
	X = execution_variable(n,Alph,Pref,D,m)
	F = final_variable(n,Alph,Pref,P,X,m)
	C = counting_variable_linear_additional_constraint(n,2*n,Alph,D,F,m)
	
	# Start Additional constraint
	# ~ for i in range(n-1):
		# ~ m.addConstr(sum([D[i][a][i] for a in Alph]) <= 2 + F[i],"one_sink_state")
	# ~ for i in range(1,n):
		# ~ m.addConstr(1 <= sum([D[j][a][i] for j in range(i) for a in Alph]),"reach")
		
	# ~ DFS = [(j,a) for j in range(n) for a in Alph]
	# ~ T = [ [ m.addVar(vtype=GRB.BINARY, name="t_%s_%s" % (l,i)) for l in range(len(DFS)) ] for i in range(n) ]
	
	# ~ for l in range(1,len(DFS)):
		# ~ m.addConstr(T[0][l] == 1,"init")
	# ~ for i in range(1,n):
		# ~ (j,a) = DFS[0]
		# ~ m.addConstr(T[i][0] == D[j][a][i],"init")
	# ~ for l in range(1,len(DFS)):
		# ~ for i in range(n):
			# ~ (j,a) = DFS[l]
			# ~ m.addConstr(T[i][l] >= D[j][a][i],"a")
			# ~ m.addConstr(T[i][l] >= T[i][l-1],"a")
			# ~ m.addConstr(T[i][l] <= T[i][l-1] + D[j][a][i],"a")
	
	# ~ for i in range(n):
		# ~ for j in range(i+1,n):
			# ~ for l in range(len(DFS)):
				# ~ m.addConstr(T[i][l] >= T[j][l],"a")
	
	# End Additional constraint
	
	m.setObjective(sum([C[i][j] for i in range(n) for j in range(2*n)]), GRB.MINIMIZE)
	m.optimize()
	# ~ for v in m.getVars():
		# ~ if (v.VarName[0] == 't'):
			# ~ print('%s %g' % (v.VarName, v.X))
	# ~ for l in P:
		# ~ print(l)
	# ~ for v in m.getVars():
		# ~ if (v.VarName[0] == 'd') or (v.VarName[0] == 'f') or (v.VarName[0] == 'N'):
			# ~ print('%s %g' % (v.VarName, v.X))
		# ~ elif (v.VarName[0] == 'W') and (v.X == 1):
			# ~ print(v.VarName)
	print('Obj: %g' % m.ObjVal)

# ~ n : number of states
def learn_language_minimal_automaton_incremental(n,nameFile):
	Alph = [x for x in range(3)]
	M = (len(Alph))**(2*n)+1

	P = read_file(nameFile)
	Pref = allPrefixes(P)
	
	test = False
	k = 1
	while (k <= n) and (test == False):		
		if (k < n):
			m1 = gp.Model("Testk")
			D1 = transition_variable(k,Alph,m1)
			X1 = execution_variable(k,Alph,Pref,D1,m1)
			F1 = final_variable(k,Alph,Pref,P,X1,m1)
			C1 = counting_variable_linear_additional_constraint(k,n+k,Alph,D1,F1,m1)
			N1 = counting_number_of_states(k,Alph,D1,F1,m1)
			m1.addConstr(N1 == k, "initialization")
			m1.setObjective(sum([C1[i][j] for i in range(k) for j in range(n+k)]), GRB.MINIMIZE)
			m1.optimize()
			val1 = m1.ObjVal
		
			m2 = gp.Model("Testn")
			D2 = transition_variable(n,Alph,m2)
			X2 = execution_variable(n,Alph,Pref,D2,m2)
			F2 = final_variable(n,Alph,Pref,P,X2,m2)
			C2 = counting_variable_linear_additional_constraint(n,n+k,Alph,D2,F2,m2)
			# ~ N2 = counting_number_of_states(k,Alph,D2,F2,m2)
			# ~ m2.addConstr(N2 >= 2, "initialization")
			m2.addConstr(sum([C2[i][j] for i in range(n) for j in range(n+k)]) <= val1-1, 'check')
			m2.optimize()
			
			if (m2.status == 3):
				test = True
				print('\n*********************************************************')
				print('For n = %s: Optimal for k = %s and obj: %g' % (n,k,val1))
				print('*********************************************************\n')
			else:
				print('\n*********************************************************')
				print('For n = %s: not optimal for k = %s' % (n,k))
				print('*********************************************************\n')
				
		else:
			m = gp.Model("Testk")
			D = transition_variable(n,Alph,m)
			X = execution_variable(n,Alph,Pref,D,m)
			F = final_variable(n,Alph,Pref,P,X,m)
			C = counting_variable_linear_additional_constraint(n,2*n,Alph,D,F,m)
			N = counting_number_of_states(k,Alph,D,F,m)
			m.addConstr(N == n, "initialization")
			m.setObjective(sum([C[i][j] for i in range(n) for j in range(2*n)]), GRB.MINIMIZE)
			m.optimize()
			val = m.ObjVal
			print('\n*********************************************************')
			print('For n = %s: Optimal for k = %s and obj: %g' % (n,k,val))
			print('*********************************************************\n')
		
		k += 1

def learn_language_minimal_automaton_nb_states(n):
	Alph = [x for x in range(3)]
	M = (len(Alph))**(2*n)+1

	nameFile = 'Traces/f:01-nw:1000-ml:02-0.words'
	P = read_file(nameFile)
	Pref = allPrefixes(P)
	
	m = gp.Model("Test")
	D = transition_variable(n,Alph,m)
	X = execution_variable(n,Alph,Pref,D,m)
	F = final_variable(n,Alph,Pref,P,X,m)
	C = counting_variable_linear_additional_constraint(n,2*n,Alph,D,F,m)
	N = counting_number_of_states(n,Alph,D,F,m)
	m.addConstr(N == n, "initialization")
	m.setObjective(sum([C[i][j] for i in range(n) for j in range(2*n)]), GRB.MINIMIZE)
	m.optimize()
	print('Obj: %g' % m.ObjVal)

# ~ learn_language_minimal_automaton_incremental(5,L[1])
# ~ learn_language_minimal_automaton(5,L[1])

learn_language_minimal_automaton(8,L[2])
# ~ learn_language_minimal_automaton(6,L[3])

# ~ learn_language_minimal_automaton(2,'Traces/TestTrace')

# ~ def main():
	# ~ l = []
	# ~ for i in range(len(L)):
		# ~ start = time.time()
		# ~ learn_language_minimal_automaton_incremental(5,L[i])
		# ~ end = time.time()
		# ~ l.append(end-start)
	# ~ print(l)

# ~ main()
# ~ start = time.time()
# ~ learn_language_minimal_automaton(5,L[16])
# ~ end = time.time()
# ~ print("Elapsed time")
# ~ print(end - start)	

# ~ l = []
# ~ for i in range(len(L)):
	# ~ nameFile = L[i]
	# ~ start = time.time()
	# ~ x = learn_language_minimal_automaton(5,nameFile)
	# ~ end = time.time()
	# ~ l.append((end-start,x))
# ~ print(l)

# ~ start = time.time()
# ~ learn_language_minimal_automaton_nb_states(6)
# ~ learn_language_minimal_automaton(6)
# ~ end = time.time()
# ~ print("Elapsed time")
# ~ print(end - start)	
	
# ~ start = time.time()
# ~ learn_language_minimal_automaton(5)
# ~ end = time.time()
# ~ print("Elapsed time")
# ~ print(end - start)

