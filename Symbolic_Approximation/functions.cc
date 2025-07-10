/**
 * @file functions.cc
 * @brief Implementaion of the functions declared in the file functions.hh
 */
 
#include <iterator>
#include <random> 
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool in_vector(vector<int> v, int n){
	for (int i : v){
		if (i == n){
			return true;
		}
	}
	return false;
}

int random_int(int n){
	random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0,n);
    
    return dist(rng);
}
