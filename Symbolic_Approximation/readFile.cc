/**
 * @file readFile.cc
 * @brief Implementaion of the functions declared in the file readFile.hh
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "readFile.hh"

using namespace std;

vector<string> readFile(string nameFile){
	vector<string> words;

	// Read from the text file
	ifstream MyReadFile(nameFile);

	// Use a while loop together with the getline() function to read the file line by line
	string myText;
	while (getline (MyReadFile, myText)) {
		words.push_back(myText);
	}

	MyReadFile.close(); 
	return words;
}
