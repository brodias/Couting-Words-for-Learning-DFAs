#include <iostream>
#include <chrono>
#include <random>

#include "functions.hh"
#include "readFile.hh"
#include "DFA.hh"
#include "learnDFA.hh"

using namespace std;

/// Returns the vector vect updated with the new score k.
/**
 * The function adds one occurence, as stored in the vector vect, of 
 * the score k. Then, returns the updated vector. 
 * 
 * @param vect a vector of pairs storing how many times each score 
 * (represented as long long unsigned int) occured.
 * @param k a score that occured once more.
 * @return the vector vect updated with the new score k that occured.
 */
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

/// Prints the vector v in descending order of the score.
/**
 * The function prints first sorts according to the score (i.e. the 
 * first components of the pair) and then display these scores (one per 
 * line), along with their number of occurrences.
 * 
 * @param vect a vector of pairs storing how many times each score 
 * (represented as long long unsigned int) occured.
 */
void print_vector_pair(vector<pair<long long unsigned int,int> > v){
	sort(v.begin(), v.end());
	cout << "********************************************" << endl;
	cout << "***** Summary of the scores obtained : *****" << endl;
	for (int i = 0; i < v.size(); i++){
		cout << "The score " << v.at(i).first << " occured " << v.at(i).second <<  " times " << endl;
	}
	cout << "********************************************" << endl;
}

/**
 * The function calls the function learnDFA argv[6] times, and keeps 
 * the DFA with the best score (given by the function eval_struct). 
 * This DFA is stored in the file given by argv[2].
 * 
 * The main function takes 6 inputs as arguments: 
 * @param argv[1] the path to the file where the list of words 
 * is stored.
 * @param argv[2] the path where we store the best DFA found.
 * @param argv[3] the number of states of the DFA.
 * @param argv[4] an integer bounding the length of words considered.
 * @param argv[5] the number of DFA structures initially (randomly) 
 * tried at each iterattions.
 * @param argv[6] the number of iterations.
 */
int main(int argc, char* argv[]) {
	string nameFile = argv[1];
	string saveFile = argv[2];
	int n = stoi(argv[3]);
	int h = stoi(argv[4]);
	int init_attempt = stoi(argv[5]);
	int iterations = stoi(argv[6]);
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
	return 0;
}
