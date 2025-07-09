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
			//cout << "Initialization, the current best is : " << best << endl;
			//cout << "Initialization, attempt number : " << k+1 << endl;
			A = B;
		}
	}
	return make_pair(best,A);
}

pair<vector<vector<vector<long long unsigned int> > >,pair<structDFA,long long unsigned int> >
	eval_one_change(structDFA A,long long unsigned int score,int horizon, vector<string> words){
	long long unsigned int best = score;
	structDFA best_struct = A;
	
	int nb_states = A.get_states();
	vector<char> Alph = A.get_Alph();
	vector<vector<int> > delta = A.get_delta();
	vector<vector<vector<long long unsigned int> > > res(nb_states);
	for (int i = 0; i < res.size(); i++){
		res.at(i) = vector<vector<long long unsigned int> >(Alph.size());
		for (int j = 0; j < Alph.size(); j++){
			res.at(i).at(j) = vector<long long unsigned int>(nb_states);
			for (int k = 0; k < nb_states; k++){
				if (delta.at(i).at(j) == k){
					res.at(i).at(j).at(k) = score;
				}
				else{
					structDFA B = A.change_transition(i,j,k);
					res.at(i).at(j).at(k) = eval_struct(B,horizon,words);
					if (res.at(i).at(j).at(k) < best){
						best = res.at(i).at(j).at(k);
						best_struct = B;
					}
				}
			}
		}
	}
	return make_pair(res,make_pair(best_struct,best));
}


pair<structDFA,long long unsigned int> simple_eval_one_change(structDFA A,long long unsigned int score,int horizon, vector<string> words){
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

pair<structDFA,long long unsigned int> eval_two_changes(structDFA A,long long unsigned int score,int horizon,vector<string> words){
	int nb_states = A.get_states();
	vector<char> Alph = A.get_Alph();
	vector<vector<int> > delta = A.get_delta();
	
	for (int i = 0; i < nb_states; i++){
		for (int j = 0; j < Alph.size(); j++){
			for (int k = 0; k < nb_states; k++){
				if (delta.at(i).at(j) != k){
					structDFA B = A.change_transition(i,j,k);
					long long unsigned int x = eval_struct(B,horizon,words);
					pair<structDFA,long long unsigned int> y = simple_eval_one_change(B,x,horizon,words);
					cout << "Score for i = " << i << ", j = " << j << ", k = " << k << " is " << y.second << endl;
					if (y.second < score){
						return y;
					}
				}
			}
		}
	}
	return make_pair(A,score);
}

pair<long long unsigned int,DFA> learnDFA_stoch(string nameFile,int n,int h,int init_attempt,int attempt,int N_proba){
	vector<string> words = readFile(nameFile);
	pair<long long unsigned int,structDFA> x = init_struct(n,h,init_attempt,words);
	cout << "The initial number of words accepted is " << x.first << endl;
	
	long long unsigned int current_score = x.first;
	structDFA current_struct = x.second;
	
	bool has_improved = true;
	int generation = 0;
	while (has_improved){
		generation++;
		pair<vector<vector<vector<long long unsigned int> > >,pair<structDFA,long long unsigned int> > 
			res = eval_one_change(current_struct,current_score,h,words);	
		vector<vector<vector<long long unsigned int> > > vect_score = res.first;
		//print_long_vect(vect_score);
		long long unsigned int best = res.second.second;
		structDFA best_struct = res.second.first;
		
		if (best < current_score){
			//cout << "Improvement!" << endl;
			//cout << "\tPrevious score : " << current_score << endl;
			//cout << "\tBetter score : " << best << endl;
			current_score = best;
			current_struct = best_struct;
			cout << "By changing one transition, we can improve to : " << best << endl;
		}
		
		else{
			//cout << "We cannot improve by changing a single transition." << endl;
			has_improved = false;
			/*
			pair<structDFA,long long unsigned int> res = eval_two_changes(current_struct,current_score,h,words);
			if (res.second < current_score){
				has_improved = true;
				current_score = res.second;
				current_struct = res.first;
				cout << "By changing two transitions, we can improve to : " << best << endl;
			}
			else{
				cout << "We cannot improve by changing two transitions." << best << endl;
				*/
			/*
			int l = 0;
			while (!has_improved && l < attempt){
				l++;
				structDFA A = best_struct;
				for (int i = 0; i < n; i++){
					for (int j = 0; j < 3; j++){
						bool b = random_bool(N_proba);
						if (b){
							vector<int> q = sort_order(vect_score.at(i).at(j));
							int new_s = random_index_ordered(q);
							A = A.change_transition(i,j,new_s);
						}
					}
				}
				long long unsigned int r = eval_struct(A,h,words);
				if (r < current_score){
					has_improved = true;
					current_score = r;
					current_struct = A;
					cout << "Generation " << generation << ", attempt : " << l << ", new best : " << current_score << endl;
				}
			}
			*/
			//}
		}
	}
	
	cout << "The final score is : " << current_score << endl;
	//cout << "The alternative score is : " << eval_struct(current_struct,15,words) << endl;
	cout << "It was found after " << generation << " genearations " << endl << endl;
	return make_pair(current_score,best_DFA(current_struct,h,words));
}
