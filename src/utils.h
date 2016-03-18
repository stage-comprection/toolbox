#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define LOGOK fprintf(stderr, "ERR : %s:%i\n", __FILE__, __LINE__);

// Generates a conversion table to compute the reverse complement of a DNA sequence
std::vector<char> generateRevCompTable();
const std::vector<char> revCompTable = generateRevCompTable();

// Computes the reverse complement of a DNA sequence using an accession table
std::string reverseComplement(const std::string& read);

// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter);

// Counts reads in a fasta file
uint countReads(std::ifstream& file);

// Sets back a file line index to 0 (getline will start from the beginning of the file again)
void resetFileIndex(std::ifstream& file);
