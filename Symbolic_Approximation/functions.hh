/**
 * @file functions.hh
 * @brief Definitions of several useful functions
 */

#ifndef functions
#define functions

#include <vector>

using namespace std;

bool in_vector(vector<int> v, int n);

bool random_bool(int N_proba);

int random_int(int n);

vector<int> sort_order(vector<long long unsigned int> vect);

int random_index_ordered(vector<int> ord);

void print_long_vect(vector<vector<vector<long long unsigned int> > > vect);

#endif
