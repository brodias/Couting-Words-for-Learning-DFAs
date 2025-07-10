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
	string nameFile = argv[1];
	cout << nameFile << endl;
	string saveFile = argv[2];
	cout << saveFile << endl;
	int n = stoi(argv[3]);
	cout << n << endl;
	int h = stoi(argv[4]);
	cout << h << endl;
	int init_attempt = stoi(argv[5]);
	cout << init_attempt << endl;
	int iterations = stoi(argv[6]);
	cout << iterations << endl;
	//~ "Traces/OrignalTraces/Traces01.words";
	//~ "DFA_stoch/DFA_01_n.dot"
	chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	vector<pair<long long unsigned int,int> > v(0);
	DFA A;
	long long unsigned int best = 0;
	for (int k = 1; k <= iterations; k++){
		pair<long long unsigned int,DFA> res = learnDFA(nameFile,n,h,init_attempt);
		v = add_vect(v,res.first);
		if (best == 0 || res.first < best){
			best = res.first;
			A = res.second;
		}
	}
	A.save(saveFile);
	chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	float time = (float) chrono::duration_cast<chrono::milliseconds>(end - begin).count()/ (float)1000;
	cout << time << "s elapsed" << std::endl;
	cout << "Number of states: " << n << std::endl;
	cout << "Horizon: " << h << std::endl;
	cout << "Number of initial attemps: " << init_attempt << std::endl;
	cout << "Number of runs: " << iterations << std::endl;
	print_vector_pair(v);
	//cout << "Done with file " + nameFile << endl;
	return 0;
}
