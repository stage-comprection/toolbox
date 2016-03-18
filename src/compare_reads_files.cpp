#include "read.h"
#include "utils.h"


// Reconstructs read vector from temporary files
void loadReadsFromFiles(std::ifstream& file1, std::ifstream& file2, std::vector<ReadPair>& reads){

    std::string line;
    uint readNumber = 0;

    while(std::getline(file1, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));
            reads[readNumber].id = readNumber;

        } else {

            reads[readNumber].seq_1 += line;
        }
    }

    std::cout<<" - File 1 loaded"<<std::endl;

    sort(reads.begin(), reads.end());

    std::cout <<" - File 1 sorted"<<std::endl;

    while(std::getline(file2, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else if (line.size() > 2){

            reads[readNumber].seq_2 += line;
        }
    }

    std::cout<<" - File 2 loaded"<<std::endl;

}



int main(int argc, char *argv[]) {

    if (argc < 4){

        std::cout<<"Usage : ./compare_reads_files file1 file2 output\n\n";
    }

    std::string file1Name = argv[1];
    std::string file2Name = argv[2];
    std::string outputFileName = argv[3];


    std::ifstream file1;
    std::ifstream file2;
    std::ofstream outputFile;

    file1.open(file1Name.c_str());
    file2.open(file2Name.c_str());

    uint file1_n = countReads(file1);
    uint file2_n = countReads(file2);

    if (file1.is_open() and file2.is_open()){

        std::vector<ReadPair> reads;
        reads.resize(std::max(file1_n, file2_n));

        loadReadsFromFiles(file1, file2, reads);

        outputFile.open(outputFileName.c_str());

        for (uint i=0; i<reads.size(); ++i){

            reads[i].compare(outputFile);
        }

        outputFile.close();

    } else {

        std::cout<<"Error : can't open one of the reads file, please check your input.\n\n";
    }

    return 0;

}
