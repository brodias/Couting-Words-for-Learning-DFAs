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

DFA DFA_from_struct_start(structDFA s,int start,vector<string> v);
		
/// Minimizes the quantity number of words up to the horizon
DFA best_DFA(structDFA s,int horizon,vector<string> v);

long long unsigned int eval_struct(structDFA s,int horizon,vector<string> v);

pair<long long unsigned int,structDFA> init_struct(int nb_states,int horizon,int nb_attempt,vector<string> words);

pair<structDFA,long long unsigned int> eval_one_change(structDFA A,long long unsigned int score,int horizon, vector<string> words);

pair<long long unsigned int,DFA> learnDFA(string nameFile,int n,int h,int init_attempt);

#endif




