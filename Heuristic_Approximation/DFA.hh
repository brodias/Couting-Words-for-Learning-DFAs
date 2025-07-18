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

/// Definition of a natrix type that store very large integer values. 
/// This allows to compute the number of words accepted by DFA up to 
/// rather large length
typedef Eigen::Matrix<long long unsigned int, Eigen::Dynamic, Eigen::Dynamic> MatrixXlli;

/// A DFA  
/**
 * This class represents a DFA by the underlying DFA structure (or 
 * transition system) along with the initial state and set of final 
 * states.
 */
class DFA{
	private:
		/// The underlying DFA structure
		structDFA s;
		
		/// The starting state
		int starting;
		
		/// The set describing the final states:
		/// F.at(i) = true <=> i is an accepting state
		vector<bool> Final;
		
	public:
		/// Returns a DFA
		/**  
		 * Note: an error arises if:
		 * - the starting state if not a valid state;
		 * - the size of F it not the number of states of sDFA.
		 * 
		 * Returns a DFA described by the input.
		 * 
		 * @param sDFA underlying DFA structure
		 * @param k the starting state
		 * @param F the set describing the final states
		 * @return the DFA described by sDFA, k, and F.
		 */
		DFA(structDFA sDFA = structDFA(),int k=0,vector<bool> F = {false});

		/**  
		 * Returns the DFA structure.
		 * @return s
		 */
		structDFA get_struct() { return s; }

		/**  
		 * Returns the starting state..
		 * @return starting
		 */
		int get_starting() { return starting; }
		
		/**  
		 * Returns the set describing the final stats.
		 * @return Final
		 */
		vector<bool> get_Final() { return Final; }
		
		/// Returns true iff the state is accepting
		/**  
		 * Note: returns an error if q is not a valid state.
		 * 
		 * Returns true iff the state q is accepting, as described by 
		 * the set Final.
		 * 
		 * @param q a state.
		 * @return true iff q is accepting
		 */
		bool is_accepting(int q);
		
		/// Save the DFA in a .dot file
		/**
		 * Save the DFA in a .dot file located at nameFile.
		 * 
		 * @param nameFile the path to where the file is saved.
		 */
		void save(string nameFile);
	
		/// Returns a Boolean vector describing which states are 
		/// reachable and co-reachable
		/**
		 * Returns a Boolean vector v such that v.at(i) = true iff
		 * the state i is reachable and co-reachable. A state q is 
		 * reachable if there is a path from starting to q. It is 
		 * co-reachable if there is a path from q to a final state. 
		 * 
		 * @return A Boolean describing which states are reachable and 
		 * co-reachable.
		 */
		vector<bool> reach_co_reach();
		
		/// Returns a transition matrix disregarding the states that are
		/// not reachable and co reachable.
		/**
		 * Returns a transition that does the following:
		 * - each state that is not reachable or co-reachable is 
		 * disregarded: the corresponding lines and columns are set to 0
		 * - a fresh self-looping state q_F is added. The other state 
		 * leading to q_F are exactly the final states. That way, the 
		 * number of words accepted up to length m is exactly the number
		 * of words of length exactly m+1 leading to q_F.
		 * 
		 * @param r_cor a boolean vector describing which states are 
		 * reachable and co-reachable
		 * @return the transition matrix disregarding the states that 
		 * are not reachable or co-reachable.
		 */
		MatrixXlli other_transition_matrix(vector<bool> r_cor);
		
		/// Returns the number of words accepted of length at most m.
		/**
		 * Returns the number of words accepted of length at most m. 
		 * This is obatined by computing the exponentiation of the 
		 * transition matrix output by the function 
		 * other_transition_matrix. We use fast exponentiation.
		 * 
		 * @param m the length of the words
		 * @return the number of words accepted of length at most m
		 */
		long long unsigned int number_of_paths(int m);
};
#endif




