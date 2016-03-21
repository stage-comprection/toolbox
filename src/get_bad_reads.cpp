#include "read.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


/* Big assumption :
 * Reads are ordered already.
 * Otherwise it won't work ..
 */

int main(int argc, char* argv[]){

    if (argc < 6) {

        std::cout << "Usage: get_bad_reads file1 file2 reads_file errors_sizes_file output_file";
    }

    std::string file1_p = argv[1];
    std::string file2_p = argv[2];
    std::string reads_p = argv[3];
    std::string errorsFile_p = argv[4];
    std::string ofile_p = argv[5];

    std::ifstream file1;
    std::ifstream file2;
    std::ifstream reads_f;
    std::ifstream errorsFile;
    std::ofstream ofile;

    file1.open(file1_p);
    file2.open(file2_p);
    reads_f.open(reads_p);
    errorsFile.open(errorsFile_p);
    ofile.open(ofile_p);

    std::string line;
    std::vector<int> errors;
    std::vector<ReadPair> reads;

    while(std::getline(errorsFile, line)){

        errors.push_back(std::stoi(line.substr(0, line.find(" : "))));
    }

//    for (uint i=0; i<100; ++i){
//        std::cout<<errors[i]<<std::endl;
//    }

    std::cout << " - Errors file loaded (" << std::to_string(errors.size()) << ")" << std::endl;

    errorsFile.close();

    std::sort(errors.begin(), errors.end());

    std::cout << " - Errors file sorted (" << std::to_string(errors.size()) << ")" << std::endl;

    ReadPair tempReadPair;
    std::vector<int>::iterator e = errors.begin();
    bool print = false;

    while(std::getline(file1, line) and e != errors.end()){

        if (line[0] == '>'){

            if (stoi(line.substr(1)) == *e){

                tempReadPair.id = *e;
                print = true;
                ++e;
            }

        } else if (print){

            tempReadPair.seq_1 = line;
            reads.push_back(tempReadPair);
            print = false;
        }

    }

    file1.close();

//    for (uint i=0; i<100; ++i){
//        std::cout<<reads[i].id<<"\n"<<reads[i].seq_1<<std::endl;
//    }

    std::cout << " - File 1 loaded (" << std::to_string(reads.size()) << ")" << std::endl;

    std::sort(reads.begin(), reads.end());

    print = false;
    e = errors.begin();
    uint r = 0;

    std::cout << " - Reads sorted (" << std::to_string(reads.size()) << ")" << std::endl;

    while (std::getline(file2, line) and e != errors.end()){

        if (line[0] == '>'){

            if (stoi(line.substr(1)) == *e){

                print = true;
//                if(r<10){
//                    std::cout<<r<<" - "<<*e<<"\n"<<line<<std::endl;
//                }
                ++e;
            }

        } else if (print) {

//            if(r<10){
//                std::cout<<r<<" - "<<*e<<"\n"<<line<<std::endl;
//            }

            reads[r].seq_2 = line;
            ++r;
            print = false;

        }
    }

//    for (uint i=0; i<100; ++i){
//        std::cout<<reads[i].id<<"\n"<<reads[i].seq_1<<"\n"<<reads[i].seq_2<<std::endl;
//    }

    print = false;
    e = errors.begin();
    r = 0;

    while (std::getline(reads_f, line) and e != errors.end()){

        if (line[0] == '>'){

            if (stoi(line.substr(1)) == *e){

                print = true;
                ++e;
            }

        } else if (print) {

            ofile << std::to_string(reads[r].id) << '\n' << reads[r].seq_1 << '\n' << reads[r].seq_2 << '\n' << line <<"\n";
            ++r;
            print = false;

        }
    }

    return 0;

}
