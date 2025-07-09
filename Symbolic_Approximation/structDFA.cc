/**
 * @file structDFA.cc
 * @brief Implementaion of the functions declared in the file structDFA.hh
 */

#include <bitset>
#include <iostream>
#include <cassert>
#include <vector>

#include "functions.hh"
#include "structDFA.hh"

using namespace std;

structDFA::structDFA(int n,vector<char> Al,vector< vector<int> > d){
	nb_states = n;
	Alph = Al;
	
	assert(d.size() == n && "Each state needs to have outgoing transitions");
	for (int i = 0; i < d.size(); i++){
		assert(d.at(i).size() == Al.size() && "Each state needs to have a transition per letter");
		for (int j = 0; j < d.at(i).size(); j++){
			assert(d.at(i).at(j) >= 0 && "States are positive");
			assert(d.at(i).at(j) <= n-1 && "States are not too large");
		}
	}
	delta = d;
	
	MatrixXlli Mat(nb_states,nb_states);
	for (int i = 0; i < nb_states; i++){
		for (int j = 0; j < nb_states; j++){
			long long unsigned int m = 0;
			for (int k = 0; k < Alph.size(); k++){
				if (delta.at(i).at(k) == j){
					m += 1;
				}
			}
			Mat(i,j) = m;
		}
	}
	M = Mat;
}

int structDFA::find_id(char c){
	int k = -1;
	for (int i = 0; i < Alph.size(); i++){
		if (c == Alph.at(i)) {
			k = i;
		}
	}
	assert((k >= 0) && "Letter not in the alphabet");
	return k;
}

int structDFA::map_to_state(int start,string w){
	int current_state = start;
	for (char& c : w){
		int k = find_id(c);
		current_state = delta.at(current_state).at(k);
	}
	return current_state;
}

structDFA structDFA::change_transition(int origin,int letter,int destination){
	vector< vector<int> > d = delta;
	d.at(origin).at(letter) = destination;
	return structDFA(nb_states,Alph,d);
}







