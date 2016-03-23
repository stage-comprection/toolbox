#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <bitset>
#include "utils.h"



// Parser for cigar string
void cigarParser(std::string& cigar, uint* output){

    // array [insertions, deletions]
    std::regex insertion_match ("(\\d+)I");
    std::regex deletion_match ("(\\d+)D");
    std::smatch matches;

    std::string cigar_copy = cigar;

    while (std::regex_search(cigar_copy, matches, insertion_match)) {

        ++output[0];
        cigar_copy = matches.suffix().str();
    }

    cigar_copy = cigar;

    while (std::regex_search(cigar_copy, matches, deletion_match)) {

        ++output[1];
        cigar_copy = matches.suffix().str();
    }
}




int main(int argc, char *argv[]) {

    if (argc < 3){

        std::cout<<"Usage: ./count_insertions input_file output_file"<<std::endl;

    } else {

        std::cout<<" Starting "<<std::endl;

        std::string inputFilePath = argv[1];
        std::string outputFilePath = argv[2];

        std::ifstream inputFile;
        std::ofstream outputFile;

        inputFile.open(inputFilePath);
        outputFile.open(outputFilePath);

        std::string line;
        std::vector<std::string> samLine;
        uint indels[2];
        std::bitset<12> flag;

        while(std::getline(inputFile, line)){

            samLine = split(line, "\t");
            flag = std::bitset<12>(std::stoi(samLine[1]));

            if (is_mapped(flag)){
                indels[0] = 0;
                indels[1] = 0;
                if (samLine[5] != "98M") cigarParser(samLine[5], indels);
                if (indels[0] != 0 or indels[1] != 0){
                    outputFile << samLine[0] << ", " << indels[0] << ", " << indels[1] << "\n";
                }
            }

        }

        inputFile.close();
        outputFile.close();
    }

}
