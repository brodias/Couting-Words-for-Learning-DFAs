/**
 * @file structDFA.hh
 * @brief Declaration of the class struct
 */

#ifndef structDFA_HH
#define structDFA_HH

#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Dense>

using namespace std;

typedef Eigen::Matrix<long long unsigned int, Eigen::Dynamic, Eigen::Dynamic> MatrixXlli;

/**
 * The class structDFA 
 */
class structDFA{
	private:
		/// Number of states (from 0 to nb_states-1)
		/// 0 is the initial state by assumption
		int nb_states;
		
		/// The alphabet
		vector<char> Alph;
		
		/// The transition function
		/// Q x Alph -> Q
		vector< vector<int> > delta;
		
		/// Transition matrix
		MatrixXlli M;
		
	public:
		structDFA(int n=1,vector<char> Al={'a','b','c'},vector<vector<int> > d={{0,0,0}});

		int get_states() { return nb_states; }
		
		vector<char> get_Alph() { return Alph; }
		
		vector< vector<int> > get_delta() { return delta; }
		
		MatrixXlli get_M() { return M; }
		
		int find_id(char c);
		
		int map_to_state(int start,string w);
		
		structDFA change_transition(int origin,int letter,int destination);
		
		structDFA extract_best_change(vector<vector<vector<long long unsigned int> > > res);
};
#endif




