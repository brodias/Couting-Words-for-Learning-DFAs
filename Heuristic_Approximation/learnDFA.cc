/**
 * @file learnDFA.cc
 * @brief Implementaion of the functions declared in the file learnDFA.hh
 */

#include "learnDFA.hh"

DFA DFA_from_struct_start(structDFA s,int start,vector<string> v){
	int nb_states = s.get_states();
	vector<bool> F = vector<bool>(nb_states,false);
	for (string word : v){
		F.at(s.map_to_state(start,word)) = true;
	}
	DFA A(s,start,F);
	return A;
}

DFA best_DFA(structDFA s,int horizon,vector<string> v){
	long long unsigned int best = 0;
	DFA A;
	int nb_states = s.get_states();
	
	for (int start = 0; start < nb_states; start++){
		DFA B = DFA_from_struct_start(s,start,v);
		long long unsigned int n = B.number_of_paths(horizon);
		if ((best == 0) || (n < best)){
			best = n;
			A = B;
		}
	}
	return A;
}

long long unsigned int eval_struct(structDFA s,int horizon,vector<string> v){
	long long unsigned int best = 0;
	int nb_states = s.get_states();
	
	for (int start = 0; start < nb_states; start++){
		DFA A = DFA_from_struct_start(s,start,v);
		long long unsigned int n = A.number_of_paths(horizon);
		if ((best == 0) || (n < best)){
			best = n;
		}
	}
	return best;
}

pair<long long unsigned int,structDFA> init_struct(int nb_states,int horizon,int nb_attempt,vector<string> words){
	long long unsigned int best = 0;
	structDFA A;
	
	for (int k = 0; k < nb_attempt; k++){
		vector<vector<int> > d = vector<vector<int> >(nb_states);
		for (int i = 0; i < d.size(); i++){
			d.at(i) = vector<int>(3);
			for (int j = 0; j < d.at(i).size(); j++){
				d.at(i).at(j) = random_int(nb_states-1);
			}
		}
		structDFA B = structDFA(nb_states,{'a','b','c'},d);
		long long unsigned int n = eval_struct(B,horizon,words);
		if ((best == 0) || (n < best)){
			best = n;
			A = B;
		}
	}
	return make_pair(best,A);
}

pair<structDFA,long long unsigned int> eval_one_change(structDFA A,long long unsigned int score,int horizon, vector<string> words){
	long long unsigned int best = score;
	structDFA best_struct = A;
	
	int nb_states = A.get_states();
	vector<char> Alph = A.get_Alph();
	vector<vector<int> > delta = A.get_delta();
	for (int i = 0; i < nb_states; i++){
		for (int j = 0; j < Alph.size(); j++){
			for (int k = 0; k < nb_states; k++){
				structDFA B = A.change_transition(i,j,k);
				long long unsigned int x = eval_struct(B,horizon,words);
				if (x < best){
					best = x;
					best_struct = B;
				}
			}
		}
	}
	return make_pair(best_struct,best);
}

pair<long long unsigned int,DFA> learnDFA(string nameFile,int n,int h,int init_attempt){
	vector<string> words = readFile(nameFile);
	pair<long long unsigned int,structDFA> x = init_struct(n,h,init_attempt,words);
	cout << "The initial number of words accepted is " << x.first << endl;
	
	long long unsigned int current_score = x.first;
	structDFA current_struct = x.second;
	
	bool has_improved = true;
	int generation = 0;
	while (has_improved){
		generation++;
		pair<structDFA,long long unsigned int> res = eval_one_change(current_struct,current_score,h,words);
		structDFA best_struct = res.first;
		long long unsigned int best = res.second;
		
		if (best < current_score){
			current_score = best;
			current_struct = best_struct;
			cout << "By changing one transition, we can improve to : " << best << endl;
		}
		
		else{
			has_improved = false;
		}
	}
	
	cout << "The final score is : " << current_score << endl;
	cout << "It was found after " << generation << " genearations " << endl << endl;
	return make_pair(current_score,best_DFA(current_struct,h,words));
}
