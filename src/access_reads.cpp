#include "utils.h"
#include "read.h"
#include <sstream>


/*
 * For sorted reads / SAM file
 */


bool is_sam(std::ifstream& file){

    std::string line;

    resetFileIndex(file);

    for (uint i=0; i<20; ++i){
        getline(file, line);
        if (line[0] == '>'){
            resetFileIndex(file);
            return false;
        }
    }

    resetFileIndex(file);
    return true;
}


int main(int argc, char *argv[]) {

    if (argc < 2){

        std::cout<<"Usage : ./access_reads file\n\n";

    } else {

        std::string fileName = argv[1];
        std::ifstream file;
        file.open(fileName);
        bool sam = is_sam(file);
        uint file_s = 0;
        std::string line;

        if (!sam){

            file_s = countReads(file);

        } else {

            uint read_n;

            while(getline(file, line)){
                if (line[0] != '@'){
                    read_n = std::stoi(line.substr(0, '\t'));
                    if (read_n > file_s) file_s = read_n;
                }
            }

            resetFileIndex(file);
        }

        std::cout << " - Reading file (";

        if(sam) std::cout << "Format : SAM) ...\n"<<std::endl; else std::cout<<"Format : not SAM) ...\n" << std::endl;

        if (file.is_open()){

            std::vector<Read> reads;
            reads.resize(file_s);

            for (uint i=0; i<reads.size(); ++i){
                reads[i].id = i;
            }

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

            sort(reads.begin(), reads.end());

            std::cout << " - File loaded\n\n" << std::endl;

            int iss_num = 0;
            std::string input = "exit";

            do {
                std::cout << "Enter read number : ";
                std::cin >> input;
                std::istringstream iss(input);

                if (!(iss >> iss_num).fail()) {

                    std::cout << ">" << reads[iss_num].id << "\n" << reads[iss_num].seq << "\n\n";

                } else if (input != "exit" and input != "quit"){

                    std::cerr << "You did not enter a valid line number. To leave the program, type 'exit' or 'quit'.\n" << std::endl;
                }

            } while (input != "exit" and input != "quit");

        } else {

            std::cout<<"Error : can't open one of the reads file, please check your input.\n\n";
        }
    }

    return 0;

}
