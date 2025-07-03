/**
 * @file DFA.hh
 * @brief Declaration of the class DFA
 */

#ifndef DFA_HH
#define DFA_HH

#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Dense>

#include "structDFA.hh"

using namespace std;

typedef Eigen::Matrix<long long unsigned int, Eigen::Dynamic, Eigen::Dynamic> MatrixXlli;

/**
 * The class DFA 
 */
class DFA{
	private:
		/// The underlying structure of the DFA
		structDFA s;
		
		/// The starting state
		int starting;
		
		/// Set of final states
		vector<bool> Final;
		
	public:
		DFA(structDFA sDFA = structDFA(),int k=0,vector<bool> F = {false});

		structDFA get_struct() { return s; }

		int get_starting() { return starting; }
		
		vector<bool> get_Final() { return Final; }
		
		bool is_accepting(int q);
		
		void save(string nameFile);
	
		vector<bool> reach_co_reach();
		
		MatrixXlli other_transition_matrix(vector<bool> r_cor);
		
		/// Returns the number of paths of length at most m from 0 to q
		long long unsigned int number_of_paths(int m);
};
#endif




