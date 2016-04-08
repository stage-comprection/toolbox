#include <iostream>
#include <fstream>


// Output length of each reads in a reads file
int main(int argc, char* argv[]){

    if( argc < 3){

        std::cerr << "Usage: get_reads_size input output" << std::endl << std::endl;

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

        std::string line, id;

        while(std::getline(inputFile, line)){

            if (line[0] != '>'){

                outputFile << id << "\t" << std::to_string(line.size()) << "\n";

            } else {

                id = line.substr(1);
            }
        }

    } else {

        if (!inputFile.is_open()){

            std::cerr << "Error : can't open the file """ << inputFilePath << """, please check your input." << std::endl;
        }

        if (!outputFile.is_open()){

            std::cerr << "Error : can't open the file """ << outputFilePath << """, please check your input." << std::endl;
        }

        return -2;
    }

    return 0;
}
