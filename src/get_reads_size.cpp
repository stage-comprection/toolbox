#include <iostream>
#include <fstream>

// Counts length of reads
void getReadsLength(std::ifstream& readFile, std::ofstream& outputFile){

    std::string line, id;

    while(std::getline(readFile, line)){

        if (line[0] != '>'){

            outputFile << id << "\t" << std::to_string(line.size()) << "\n";

        } else {

            id = line.substr(1);
        }
    }
}


int main(int argc, char* argv[]){

    if( argc < 3){

        std::cout<<"Usage: get_reads_size input output\n\n";
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    std::ifstream inputFile;
    std::ofstream outputFile;

    inputFile.open(inputFileName.c_str());
    outputFile.open(outputFileName.c_str());

    getReadsLength(inputFile, outputFile);

    inputFile.close();
    outputFile.close();

    return 0;
}
