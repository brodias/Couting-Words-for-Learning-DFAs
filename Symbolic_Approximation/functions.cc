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

/// Returns true with proba 1/N_proba
bool random_bool(int N_proba){
	random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(1,N_proba);
    return (dist(rng) == N_proba);
}

int random_int(int n){
	random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0,n);
    
    return dist(rng);
}

vector<int> sort_order(vector<long long unsigned int> vect){
	vector<long long unsigned int> v = vect;
	sort( v.begin(), v.end() );
	v.erase( unique( v.begin(), v.end() ), v.end() );
	
	vector<int> res = vector<int>(vect.size());
	for (int k = 0; k < res.size(); k++){
		res.at(k) = find(v.begin(), v.end(), vect.at(k)) - v.begin();
	}
	
	return res;
}

int random_index_ordered(vector<int> ord){
/*	int n = ord.size();
	int m = *max_element(ord.begin(), ord.end());
	int M = (m+1) * (m+2)/2;
	
	random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0,M);
    
    int r = dist(rng);
    for (int k = 0; k <= m; k++){
		if (r < (k+1) * (k+2) /2){
			vector<int> d;
			for (int i = 0; i < ord.size(); i++){
				if (ord.at(i) == k){
					d.push_back(i);
				}
			}
			return d.at(random_int(d.size()-1));
		}
	}
	return 0;*/
	return ord.at(random_int(ord.size()-1));
}

void print_long_vect(vector<vector<vector<long long unsigned int> > > vect){
	vector<char> Alph = {'a','b','c'};
	for (int i = 0; i < vect.size(); i++){
		cout << "From state " << i << " : " << endl;
		for (int j = 0; j < vect.at(i).size(); j++){
			cout << "\tLetter " << Alph.at(j) << " : " << endl;
			for (int k = 0; k < vect.at(i).at(j).size(); k++){
				cout << "\t\tTo state " << k << " : " << vect.at(i).at(j).at(k) << endl;
			}
		}
	}
}
