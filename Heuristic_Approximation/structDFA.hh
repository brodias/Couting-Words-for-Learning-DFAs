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

/// A DFA structures given by states and transitions 
/**
 * This class represents a DFA structure (or a transition) described by 
 * the set of states, alphabet, and transitions between states
 */
class structDFA{
	private:
		/// Number of states (from 0 to nb_states-1)
		int nb_states;
		
		/// The alphabet
		vector<char> Alph;
		
		/// The transition function
		/// Q x Alph -> Q, so delta.at(i).size() = Alph.size()
		vector< vector<int> > delta;
		
		/// Transition matrix that is computed at initialization
		/// M.at(i).at(j) = |{ a \in Alph | \delta(q_i,a) = q_j }|
		MatrixXlli M;
		
	public:
		/// Returns a DFA structure
		/**  
		 * Note: an error arises if:
		 * - the size of d if not n; or
		 * - for some i, the size of d.at(i) is not Al.size(); or
		 * - for some i and some j d.at(i).at(j) is not a valid state.
		 * 
		 * Returns a DFA structure described by the input. The 
		 * transition matrix is computed at initialization.
		 * 
		 * @param n the number of state.
		 * @param Al the alphabet
		 * @param d the transition function.
		 * @return the DFA structure described by n, Al, and d.
		 */
		structDFA(int n=1,vector<char> Al={'a','b','c'},vector<vector<int> > d={{0,0,0}});

		/**  
		 * Returns the number of states.
		 * @return nb_states
		 */
		int get_states() { return nb_states; }
		
		/**  
		 * Returns the alphabet.
		 * @return Alph
		 */
		vector<char> get_Alph() { return Alph; }
		
		/**  
		 * Returns the transition function.
		 * @return delta
		 */
		vector< vector<int> > get_delta() { return delta; }
		
		/**  
		 * Returns the transition matrix.
		 * @return M
		 */
		MatrixXlli get_M() { return M; }
		
		/// Returns an int corresponding the character c in Alph
		/**  
		 * Returns an int j such that Alph.at(j) = c if one exists. 
		 * Otherwise, returns an error.
		 * 
		 * @param c a letter in the alphabet.
		 * @return an int j such that Alph.at(j) = c
		 */
		int find_id(char c);
		
		/// Returns the state that the word w is mapped to from the 
		/// starting state state start
		/**  
		 * Note: returns an error if start is not a valid state.
		 * 
		 * Returns the state (int <= nb_states - 1) j equal to
		 * j = \delta(start,w)
		 * 
		 * @param start the initial state.
		 * @param c a letter in the alphabet.
		 * @return an int j = \delta(start,w)
		 */
		int map_to_state(int start,string w);
		
		/// Returns a DFA structure obtained by changing a single 
		/// transition
		/**  
		 * Note: returns an error if:
		 * - origin is not a valid state; or
		 * - letter is not a valid letter; or
		 * - destination is not a valid state.
		 * 
		 * Returns a DFA structure obatained by changing a single 
		 * transitionthe: d(origin,letter) = destination
		 * 
		 * @param origin the state from which the transition to change 
		 * starts.
		 * @param letter the letter labelling the transition to change.
		 * @param destination the state to which the transition to 
		 * change goes.
		 * @return the DFA structure obtained by changing the transition 
		 * delta by \delta(origin,letter) = destination
		 */
		structDFA change_transition(int origin,int letter,int destination);
};
#endif




