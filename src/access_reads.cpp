#include "utils.h"
#include "read.h"


/*
 * For sorted reads / SAM file
 */


bool is_sam(std::ifstream& file){

    std::string line;

    resetFileIndex(file);

    for (uint i=0; i<100; ++i){
        getline(file, line);
        if (line[0] == '>') return false;
    }

    return true;
}


int main(int argc, char *argv[]) {

    if (argc < 2){

        std::cout<<"Usage : ./access_reads file\n\n";

    } else {

        std::string fileName = argv[1];
        std::ifstream file;
        file.open(fileName);
        uint file_s = countReads(file);

        if (file.is_open()){

            std::vector<Read> reads;
            reads.resize(file_s);

            for (uint i=0; i<reads.size(); ++i){
                reads[i].id = i;
            }

            bool sam = is_sam(file);

            std::string line;
            uint readNumber = 0;

            if (sam){

                std::vector<std::string> s_line;

                while(std::getline(file, line)){

                    s_line = split(line, "\t");
                    reads[std::stoi(s_line[0])].seq = line;
                }

            } else {

                while(std::getline(file, line)){

                    if (line[0] == '>'){

                        readNumber = std::stoi(line.substr(1));
                        reads[readNumber].id = readNumber;

                    } else {

                        reads[readNumber].seq += line;
                    }
                }
            }

            file.close();

        } else {

            std::cout<<"Error : can't open one of the reads file, please check your input.\n\n";
        }
    }

    return 0;

}
