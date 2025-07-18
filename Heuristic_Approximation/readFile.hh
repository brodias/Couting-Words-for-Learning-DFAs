/**
 * @file readFile.hh
 * @brief Function for reading file
 */

#ifndef rFile_HH
#define rFile_HH

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/// Returns the content of file located at 'nameFile'
/**  
 * Returns the content of file located at 'nameFile' as a vector of 
 * one string per line
 * 
 * @param nameFile the location of the file to read.
 * @return the content of the file as a string vector.
 */
vector<string> readFile(string nameFile);

#endif




