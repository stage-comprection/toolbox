#include "read.h"
#include "utils.h"


// Sort a read file
int main(int argc, char* argv[]){

    if( argc < 3){

        std::cerr << "Usage : ./sort_reads input output" << std::endl << std::endl;

        return -1;
    }

    // Retrieve command line arguments
    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    // Initialize file streams
    std::ifstream inputFile;
    std::ofstream outputFile;

    // Open files
    inputFile.open(inputFilePath.c_str());
    outputFile.open(outputFilePath.c_str());

    if (inputFile.is_open() and outputFile.is_open()){

        uint inputFile_n = countReads(inputFile); // Count number of reads in file

        // Initialize and empty Reads vector of size = number of reads
        std::vector<Read> reads;
        reads.resize(inputFile_n);

        // Initialize reads ID  = position of the read in the file (read1 = 0, read2 = 1 ...)
        for (uint i=0; i<reads.size();++i){
            reads[i].id = i;
        }

        std::string line;
        uint readNumber = 0;

        // Load reads sequence
        while(std::getline(inputFile, line)){

            if (line[0] == '>'){

                readNumber = stoi(line.substr(1));

            } else {

                reads[readNumber].seq += line;
            }
        }

        sort(reads.begin(), reads.end()); // Sorting according to ID

        // Generate output file
        for (uint i=0; i<reads.size(); ++i){

            outputFile << ">" << std::to_string(reads[i].id) << "\n" << reads[i].seq << "\n";
        }

        outputFile.close();

    } else {

        if (!inputFile.is_open()){

            std::cerr << "Error : can't open the file """ << inputFilePath << """, please check your input." << std::endl;
        }

        if (!outputFile.is_open()){

            std::cerr << "Error : can't open the file """ << outputFilePath << """, please check your input." << std::endl;
        }

        return -2;
    }
}
