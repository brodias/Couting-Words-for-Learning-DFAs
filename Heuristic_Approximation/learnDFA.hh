/**
 * @file learnDFA.hh
 * @brief Two functions to learn a DFA from positive examples only
 */

#ifndef learn_DFA
#define learn_DFA

#include "functions.hh"
#include "readFile.hh"
#include "structDFA.hh"
#include "DFA.hh"

#include <vector>

using namespace std;

/// Returns a DFA accepting all the words described in v
/**
 * Returns a DFA with s as DFA structure, start as starting state, and 
 * such that a state q is final if and only if a word in v is mapped to 
 * q from the initial state start.
 * 
 * @param s a DFA structure.
 * @param start the starting state.
 * @param v a vector of words.
 * @return A DFA built from s and start that accepts all words in v.
 */
DFA DFA_from_struct_start(structDFA s,int start,vector<string> v);
		
/// Returns a DFA that accepts the least number of words of length at 
/// most horizon while accepting all words in v. 
/**
 * Returns a DFA hat minimizes the number of words accepted of length 
 * at most horizon among those DFAs that accept all words in v whose 
 * structure is s. This is done enumerating over all starting state, 
 * considering the DFA given by the function DFA_from_struct_start, 
 * and then computing the number of words accepted.
 * 
 * @param s a DFA structure.
 * @param horizon an integer bounding the length of words considered.
 * @param v a vector of words.
 * @return A DFA built from s that minimizes the number of words 
 * accepted of length at most horizon among those DFAs that accept
 * all words in v.
 */
DFA best_DFA(structDFA s,int horizon,vector<string> v);

/// Returns the least number of words of length at most horizon 
/// accepted by a DFA accepting all words in v. 
/**
 * Returns the least number of words accepted of length at most horizon 
 * by DFAs that accept all words in v whose structure is s. This 
 * function works exactly like the function best_DFA, except that it 
 * returns the number of words, instead of the DFA.
 * 
 * @param s a DFA structure.
 * @param horizon an integer bounding the length of words considered.
 * @param v a vector of words.
 * @return the least number of words of length at most horizon 
 * accepted by a DFA accepting all words in v. 
 */
long long unsigned int eval_struct(structDFA s,int horizon,vector<string> v);

/// Returns the pair of the best DFA structure and its score after 
/// having tried randomly nb_attempt DFA structures. 
/**
 * Among nb_attempt DFA structures randomly looked at, pick one with 
 * the best score (given by the function eval_struct). Returns the pair 
 * of this DFA structure and its score.
 * 
 * @param nb_states the number of states of the DFA structures.
 * @param horizon an integer bounding the length of words considered.
 * @param nb_attempt the number of DFA structures considered (randomly).
 * @return a pair of the best DFA structure found and its score. 
 */
pair<long long unsigned int,structDFA> init_struct(int nb_states,int horizon,int nb_attempt,vector<string> words);

/// Returns the pair of the best DFA structure and its score after 
/// having tried to change a single transition. 
/**
 * Starting from the DFA structure A, looks for an improvement of the 
 * score (given by the function eval_struct) by changing a single 
 * transition. All possible changes are tried.
 * 
 * @param A a DFA structure.
 * @param score a score (integer) to improve by changing a transition.
 * @param horizon an integer bounding the length of words considered.
 * @param words a vector of words.
 * @return a pair of the best DFA structure and its score after
 * having tried to change a single transition. 
 */
pair<structDFA,long long unsigned int> eval_one_change(structDFA A,long long unsigned int score,int horizon, vector<string> words);

/// Returns a pair of the best DFA structure found and its score. 
/**
 * Starts from the DFA structure given by the function init_struct. 
 * Then, while this improves the score of the DFA structure, 
 * iteratively change a single transition (given by the function 
 * eval_one_change). When no improvement can be made to the score by 
 * changing a single transition, the current DFA structure is returned, 
 * along with its score (given by the function eval_struct).
 * 
 * @param nameFile the path to the file where the list of words is 
 * stored.
 * @param n the number of states of the DFA.
 * @param h an integer bounding the length of words considered.
 * @param init_attempt the number of DFA structures initially 
 * (randomly) tried.
 * @return a pair of the best DFA structure found and its score. 
 */
pair<long long unsigned int,DFA> learnDFA(string nameFile,int n,int h,int init_attempt);

#endif




