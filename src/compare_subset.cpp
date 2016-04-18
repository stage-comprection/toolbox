#include "read.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


/* All files should already be sorted
 * This should be true if using my other tools I think.
 */

int main(int argc, char* argv[]){

    if (argc < 4) {

        std::cerr << "Usage: ./compare_subset file1 file2 output_file" << std::endl << std::endl;

        return -1;
    }

    // Retrieve command line arguments
    std::string file1_p = argv[1];
    std::string file2_p = argv[2];
    std::string ofile_p = argv[3];
    std::string ofile_count_p = ofile_p + "_sizes";

    // Initialize file streams
    std::ifstream file1;
    std::ifstream file2;
    std::ofstream ofile;
    std::ofstream ofile_count;

    // Open files
    file1.open(file1_p);
    file2.open(file2_p);
    ofile.open(ofile_p);
    ofile_count.open(ofile_count_p);

    if (file1.is_open() and file2.is_open() and ofile.is_open() and ofile_count.is_open()){

        uint file1_s = countReads(file1);
        uint file2_s = countReads(file2);

        std::ifstream file;
        if (file1_s > file2_s) file.open(file2_p);
        else file.open(file1_p);

        std::string line;
        std::vector<ReadPair> reads;

        // Sequences from files 1 and 2 for reads with errors are stored in ReadPair object
        ReadPair tempReadPair;

        while(std::getline(file, line)){

            if (line[0] == '>'){

                tempReadPair.id = std::stoi(line.substr(1));

            } else {

                tempReadPair.seq_1 = line;
                reads.push_back(tempReadPair);
            }
        }

        std::sort(reads.begin(), reads.end());

        file.close();
        std::cout << " - File 1 loaded (" << std::to_string(reads.size()) << " reads)." << std::endl;

        bool print;
        std::vector<ReadPair>::iterator it = reads.begin();

        if (file1_s > file2_s) file.open(file1_p);
        else file.open(file2_p);

        uint r = 0, t = 0;

        // Store sequence for all reads with errors from file 2 (same as file 1)
        while (std::getline(file, line) and it != reads.end()){

            if (line[0] == '>'){

                t = stoi(line.substr(1));

                if (t == it->id){

                    print = true;
                }

            } else if (print) {

                reads[r].seq_2 = line;
                ++r;
                ++it;
                print = false;
            }
        }

        file.close();
        std::cout << " - File 2 loaded." << std::endl;

        for (uint i=0; i<reads.size();++i){

            reads[i].compare(ofile, ofile_count);
        }

    } else {

        if (!file1.is_open()){

            std::cerr << "Error : can't open the file """ << file1_p << """, please check your input." << std::endl;
        }

        if (!file2.is_open()){

            std::cerr << "Error : can't open the file """ << file2_p << """, please check your input." << std::endl;
        }

        if (!ofile.is_open()){

            std::cerr << "Error : can't open the file """ << ofile_p << """, please check your input." << std::endl;
        }

        if (!ofile_count.is_open()){

            std::cerr << "Error : can't open the file """ << ofile_count_p << """, please check your input." << std::endl;
        }

        return -2;
    }

    return 0;

}
