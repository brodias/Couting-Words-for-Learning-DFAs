#include <iostream>
#include <chrono>
#include <random>

#include "functions.hh"
#include "readFile.hh"
#include "DFA.hh"
#include "learnDFA.hh"

using namespace std;

vector<pair<long long unsigned int,int> > add_vect(vector<pair<long long unsigned int,int> > vect,long long unsigned int k){
	vector<pair<long long unsigned int,int> > v = vect;
	bool added = false;
	for (int i = 0; i < v.size(); i++){
		if (v.at(i).first == k){
			added = true;
			v.at(i) = make_pair(k,v.at(i).second+1);
		}
	}
	if (!added){
		v.push_back(make_pair(k,1));
	}
	return v;
}

void print_vector_pair(vector<pair<long long unsigned int,int> > v){
	sort(v.begin(), v.end());
	cout << "********************************************" << endl;
	cout << "***** Summary of the scores obtained : *****" << endl;
	for (int i = 0; i < v.size(); i++){
		cout << "The score " << v.at(i).first << " occured " << v.at(i).second <<  " times " << endl;
	}
	cout << "********************************************" << endl;
}

int main(int argc, char* argv[]) {
	int i = stoi(argv[1]);
	int n = stoi(argv[2]);
	int h = stoi(argv[3]);
	int init_attempt = stoi(argv[4]);
	int iterations = stoi(argv[5]);
	
	int attempt = 10;
	int N_proba = 10;
	string s;
	if (i < 10){
		s = "0" + to_string(i);
	}
	else{
		s = to_string(i);
	}
	string nameFile = "../Traces/Traces" + s + ".words";
	chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	vector<pair<long long unsigned int,int> > v(0);
	DFA A;
	long long unsigned int best = 0;
	for (int k = 1; k <= iterations; k++){
		pair<long long unsigned int,DFA> res = learnDFA_stoch(nameFile,n,h,init_attempt,attempt,N_proba);
		v = add_vect(v,res.first);
		if (best == 0 || res.first < best){
			best = res.first;
			A = res.second;
		}
	}
	A.save("DFA_stoch/DFA_" + s + "_" + to_string(n) + ".dot");
	chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	cout << "Elapsed time = " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " s " << std::endl;
	print_vector_pair(v);
	//cout << "Done with file " + nameFile << endl;
	return 0;
}
