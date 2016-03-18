#include "read.h"
#include "utils.h"

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

    uint inputFile_n = countReads(inputFile);

    if (inputFile.is_open() and outputFile.is_open()){

        std::vector<Read> reads;
        reads.resize(inputFile_n);

        std::string line;
        uint readNumber = 0;

        while(std::getline(inputFile, line)){

            if (line[0] == '>'){

                readNumber = stoi(line.substr(1));
                reads[readNumber].id = readNumber;

            } else {

                reads[readNumber].seq += line;
            }
        }

        sort(reads.begin(), reads.end());

        outputFile.open(outputFileName.c_str());

        for (uint i=0; i<reads.size(); ++i){

            outputFile << ">" << reads[i].id << "\n" << reads[i].seq << "\n";
        }

        outputFile.close();

    } else {

        std::cout<<"Error : can't open one of the reads file, please check your input.\n\n";
    }
}
