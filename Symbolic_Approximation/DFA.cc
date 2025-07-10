/**
 * @file DFA.cc
 * @brief Implementaion of the functions declared in the file DFA.hh
 */

#include <bitset>
#include <iostream>
#include <cassert>
#include <vector>

#include "functions.hh"
#include "DFA.hh"
#include "structDFA.hh"

using namespace std;

DFA::DFA(structDFA sDFA,int k,vector<bool> F){
	int n = sDFA.get_states();
	assert((k >= 0) && "Not a valid state");
	assert((k <= n-1) && "Not a valid state");
	assert((F.size() == n) && "Each state is accepting or not accepting");
	
	s = sDFA;
	starting = k;
	Final = F;
}

bool DFA::is_accepting(int q){
	assert(q >= 0 && "Not a valid state");
	assert(q <= s.get_states()-1 && "Not a valid state");
	return Final.at(q);
}

void DFA::save(string nameFile){
	// Create and open a text file
	ofstream File(nameFile);
	
	int nb_states = s.get_states();
	vector<char> Alph = s.get_Alph();
	vector<vector<int> > delta = s.get_delta();

	// Write in the file
	// We follow the .dot grammar
	
	File << "digraph g {" << endl;
	File << "__start [label=" <<'"' << '"' << "shape=none]" << endl;
	string circ;
	
	// We start by writting the set of states
	// The first state written is the initial state
	// 'doublecircle' means that the state is final
	// 'circle' means that the state is not final
	if (is_accepting(starting)){
		circ = "doublecircle";
	}
	else{
		circ = "circle";
	}
	File << "\t" << starting << "[shape=" << '"' << circ << '"' << " label=" << '"' << starting << '"' << "]" << endl;
	for (int i = 0; i < nb_states; i++){
		string circ;
		if (i != starting){
			if (is_accepting(i)){
				circ = "doublecircle";
			}
			else{
				circ = "circle";
			}
			File << "\t" << i << "[shape=" << '"' << circ << '"' << " label=" << '"' << i << '"' << "]" << endl;
		}
	}
	File << endl;
	
	// We then write the transitions 
	File << "__start -> " << starting << endl;
	for (int i = 0; i < nb_states; i++){
		for (int j = 0; j < Alph.size(); j++){
			File << "\t" << i << " -> " << delta.at(i).at(j) << " [label=" << '"' << Alph.at(j) << '"' << "]" << endl;
		}
	}
	File << "}" << endl;

	// Close the file
	File.close();
}

vector<bool> DFA::reach_co_reach(){
	int nb_states = s.get_states();
	vector<vector<int> > delta = s.get_delta();
	
	// We iteratively compute the set of states that are reachable.
	// Initially, only the starting state is deemed reachable.
	vector<bool> reach = vector<bool>(nb_states);
	for (int i = 0; i < reach.size(); i++){
		reach.at(i) = (i == starting);
	}
	
	// Then, at step n+1, we deem a state q reachable if:
	// - q was deemed reachable at step n; or
	// - some state q' was deemed reachable at step n and there is a 
	// transition from q' to q.
	for (int n = 1; n < nb_states; n++){
		for (int i = 0; i < reach.size(); i++){
			if (reach.at(i)){
				for (int j = 0; j < delta.at(i).size(); j++){
					reach.at(delta.at(i).at(j)) = true;
				}
			}
		}
	}
	
	// This is symmetrical with co-reachable states: initially only the 
	// the final states are demmed co-reachable, etc.
	vector<bool> co_reach = vector<bool>(nb_states);
	for (int i = 0; i < co_reach.size(); i++){
		co_reach.at(i) = is_accepting(i);
	}
	for (int n = 1; n < nb_states; n++){
		for (int i = 0; i < co_reach.size(); i++){
			bool b = co_reach.at(i);
			for (int j = 0; j < delta.at(i).size(); j++){
				if (co_reach.at(delta.at(i).at(j))){
					b = true;
				}
			}
			co_reach.at(i) = b;
		}
	}
	
	// Finally, we compute the set of states that are reachable AND 
	// co-reachable.
	for (int i = 0; i < co_reach.size(); i++){
		co_reach.at(i) = co_reach.at(i) && reach.at(i);
	}
	return co_reach;
}

MatrixXlli DFA::other_transition_matrix(vector<bool> r_cor) {
	int nb_states = s.get_states();
	assert((r_cor.size() == nb_states) && "Wrong size of the reach,co-reach vector.");
	
	MatrixXlli M = s.get_M();
	
	MatrixXlli Macc(nb_states+1,nb_states+1);
	Macc(nb_states,nb_states) = 1;
	for (int i = 0; i < nb_states; i++){
		// Exactly the accepting state have a transition to the 
		// additional fresh state q_F
		if (is_accepting(i)){
			Macc(i,nb_states) = 1;
		}
		else {
			Macc(i,nb_states) = 0;
		}
		Macc(nb_states,i) = 0;
		for (int j = 0; j < nb_states; j++){
			// We disregard the states that are not reacheble or not 
			// co-reachable.
			if (!r_cor.at(i) || !r_cor.at(j)){
				Macc(i,j) = 0;
			}
			else {
				Macc(i,j) = M(i,j);
			}
		}
	}
	return Macc;
}

long long unsigned int DFA::number_of_paths(int m){
	int nb_states = s.get_states();
	vector<bool> r_cor = reach_co_reach();
	MatrixXlli Macc = other_transition_matrix(r_cor);
	
	// We use the bit representation for fast exponentiation.
	bitset<32> x(m+1);
	string v = x.to_string();
	int start = 0;
	while (v[start] == '0'){
		start += 1;
	}
	
	// Fast exponentiation.
	MatrixXlli CurrentM = Macc;
	for (int i = start+1; i < v.size(); i++){
		CurrentM = CurrentM  * CurrentM;
		if (v[i] == '1'){
			CurrentM = CurrentM * Macc;
		}
	}	

	return CurrentM(starting,nb_states);
}






