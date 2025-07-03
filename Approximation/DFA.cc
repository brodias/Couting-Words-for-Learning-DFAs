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
	s = sDFA;
	starting = k;
	Final = F;
}

bool DFA::is_accepting(int q){
	assert(q >= 0 && "States are positive");
	assert(q <= s.get_states()-1 && "States are not too large");
	return Final.at(q);
}

void DFA::save(string nameFile){
	// Create and open a text file
	ofstream File(nameFile);
	
	int nb_states = s.get_states();
	vector<char> Alph = s.get_Alph();
	vector<vector<int> > delta = s.get_delta();

	// Write to the file
	File << "digraph g {" << endl;
	File << "__start [label=" <<'"' << '"' << "shape=none]" << endl;
	string circ;
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
	
	vector<bool> reach = vector<bool>(nb_states);
	for (int i = 0; i < reach.size(); i++){
		reach.at(i) = (i == starting);
	}
	for (int n = 1; n < nb_states; n++){
		for (int i = 0; i < reach.size(); i++){
			if (reach.at(i)){
				for (int j = 0; j < delta.at(i).size(); j++){
					reach.at(delta.at(i).at(j)) = true;
				}
			}
		}
	}
	
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
	
	for (int i = 0; i < co_reach.size(); i++){
		co_reach.at(i) = co_reach.at(i) && reach.at(i);
	}
	return co_reach;
}

MatrixXlli DFA::other_transition_matrix(vector<bool> r_cor) {
	int nb_states = s.get_states();
	MatrixXlli M = s.get_M();
	
	MatrixXlli Macc(nb_states+1,nb_states+1);
	Macc(nb_states,nb_states) = 1;
	for (int i = 0; i < nb_states; i++){
		if (is_accepting(i)){
			Macc(i,nb_states) = 1;
		}
		else {
			Macc(i,nb_states) = 0;
		}
		Macc(nb_states,i) = 0;
		for (int j = 0; j < nb_states; j++){
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
	
	bitset<32> x(m+1);
	string v = x.to_string();
	int start = 0;
	while (v[start] == '0'){
		start += 1;
	}
	// cout << "The binary decomposition of " << m+1 << " is equal to : " << endl;
	// cout << x << endl << endl;
	// cout << "The first bit equal to 1 is equal to : " << endl;
	// cout << start << endl << endl;
	
	/// The fast exponentiation is done with 'decalage d'index' (on fait le carre au debut)
	MatrixXlli CurrentM = Macc;
	for (int i = start+1; i < v.size(); i++){
		CurrentM = CurrentM  * CurrentM;
		if (v[i] == '1'){
			CurrentM = CurrentM * Macc;
		}
	}	
	//cout << "The Matrix for power " << m+1 << " is equal to : " << endl;
	//cout << CurrentM << endl << endl;
	return CurrentM(starting,nb_states);
}






