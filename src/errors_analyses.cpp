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

    if (argc < 5) {

        std::cout << "Usage: errors_analyses file1 file2 errorsFile output";
    }

    std::string file1_p = argv[1];
    std::string file2_p = argv[2];
    std::string errorsFile_p = argv[3];
    std::string ofile_p = argv[4];

    std::ifstream file1;
    std::ifstream file2;
    std::ifstream errorsFile;
    std::ofstream ofile;

    file1.open(file1_p);
    file2.open(file2_p);
    errorsFile.open(errorsFile_p);
    ofile.open(ofile_p);

    std::string line;
    std::vector<int> errors;
    std::vector<Read> reads;

    while(std::getline(errorsFile, line)){

        errors.push_back(std::stoi(line.substr(0, line.find('\t'))));
    }

    errorsFile.close();

    std::sort(errors.begin(), errors.end());

    Read tempRead;
    std::vector<int>::iterator e = errors.begin();
    bool print = false;

    while(std::getline(file1, line) and e != errors.end()){

        if (line[0] == '>'){

            if (stoi(line.substr(1)) == *e){

                tempRead.id = *e;
                print = true;
                ++e;
            }

        } else if (print){

            tempRead.seq = line;
            reads.push_back(tempRead);
            print = false;
        }

    }

    file1.close();

    std::sort(reads.begin(), reads.end());

    e = errors.begin();
    uint r = 0;


    while (std::getline(file2, line) and e != errors.end()){

        if (line[0] == '>'){

            if (stoi(line.substr(1)) == *e){

                print = true;
                ++e;
            }

        } else if (print) {

            ofile << std::to_string(reads[r].id) << '\n' << reads[r].seq << '\n' << line <<'\n';
            ++r;
            print = false;

        }
    }

    return 0;

}
