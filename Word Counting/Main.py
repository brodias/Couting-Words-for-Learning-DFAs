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
				for a in range(len(Alph)) ] 
				for i in range(n)]

	# Unique transition
	for i in range(n):
		for a in range(len(Alph)): 
			m.addConstr(sum([ D[i][a][j] for j in range(n)]) == 1, "unique transition")
	
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
					for a in range(len(Alph)):
						if Pref[l2] == Pref[l1] + [Alph[a]]:
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
def counting_variable_linear(n,l,Alph,D,F,m): 
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
			for a in range(len(Alph)) ]
			for i1 in range(n) ]

	# Add constraint : "I[i1][a][i2][j] = N[i1][j] * D[i1][a][i2]"
	for i1 in range(n):
		for a in range(len(Alph)):
			for i2 in range(n):
				for j in range(l-1):
					m.addConstr(N[i1][j] + D[i1][a][i2] * M <= I[i1][a][i2][j] + M, "intermediate_step")
					m.addConstr(I[i1][a][i2][j] <= D[i1][a][i2] * M, "intermediate_step")
					m.addConstr(0 <= I[i1][a][i2][j], "intermediate_step")
			
	for i2 in range(n):	
		for j in range(1,l):
			m.addConstr(N[i2][j] == sum([I[i1][a][i2][j-1] for i1 in range(n) for a in range(len(Alph))]),"update")

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
	Alph = []
	with open(nameFile) as f: s = f.read()
	L = []
	i = 0
	while (i < len(s)) and (s[i] != '-'):
		l = []
		while (i < len(s)) and (s[i] != '\n'):
			if (not s[i] in Alph):
				Alph += s[i]
			l += s[i]
			i += 1
		if (l != []):
			L.append(l)
		i += 1
	return (L,Alph)

L = [	'../Traces/Traces01.words',
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
	(P,Alph) = read_file(nameFile)
	
	M = (len(Alph))**(2*n)+1
	Pref = allPrefixes(P)
	
	m = gp.Model("Test")
	# ~ m.setParam('TimeLimit', 30)
	D = transition_variable(n,Alph,m)
	X = execution_variable(n,Alph,Pref,D,m)
	F = final_variable(n,Alph,Pref,P,X,m)
	C = counting_variable_linear(n,2*n-1,Alph,D,F,m)
	
	# Start Additional constraint
	# ~ for i in range(n-1):
		# ~ m.addConstr(sum([D[i][a][i] for a in range(len(Alph))]) <= 2 + F[i],"one_sink_state")
	# ~ for i in range(1,n):
		# ~ m.addConstr(1 <= sum([D[j][a][i] for j in range(i) for a in range(len(Alph))]),"reach")
		
	# ~ DFS = [(j,a) for j in range(n) for a in range(len(Alph))]
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
	
	# ~ if (n >= 2):
		# ~ m.addConstr(D[0][0][0] + D[0][0][1] == 1, "unique transition")
	# ~ if (n >= 3):
		# ~ m.addConstr(D[0][1][0] + D[0][1][1] + D[0][1][2] == 1, "unique transition")
	# ~ if (n >= 4):
		# ~ m.addConstr(D[0][2][0] + D[0][2][1] + D[0][2][2] + D[0][2][3] == 1, "unique transition")
	
	# End Additional constraint
	
	m.setObjective(sum([C[i][j] for i in range(n) for j in range(2*n-1)]), GRB.MINIMIZE)
	m.optimize()
	print('Obj: %g' % m.ObjVal)

learn_language_minimal_automaton(7,L[0])

