#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>



// Format a reads file to give to each read an ID = position of the read in file (read 1 = 0, read 2 = 1 ...) and discard reads with Ns.
int main(int argc, char *argv[]){

    if (argc < 2){

        std::cerr << "Usage : ./format_reads_file file (keep_N)" << std::endl << std::endl;

        return -1;

    } else {

        // Retrieve command-line arguments
        std::string inputFilePath = argv[1];
        std::string backupFilePath = inputFilePath + ".backup";

        int rename_res = std::rename(inputFilePath.c_str(), backupFilePath.c_str());

        if (rename_res != 0){

            std::cerr << "Error : could not rename the file """ << inputFilePath << """ into """ << backupFilePath <<
                         """.\nCheck input file name." << std::endl;

            return -3;

        } else {

            // Initialize file streams
            std::ifstream inputFile;
            std::ofstream outputFile;

            inputFile.open(backupFilePath);
            outputFile.open(inputFilePath);

            if (inputFile.is_open() and outputFile.is_open()){

                // If keep_N is false, reads with N in their sequence will be discarded
                bool keep_N = false;

                if (argc == 3){

                    std::string temp = argv[2];
                    if (temp == "TRUE" or temp == "True" or temp == "true" or temp == "T" or temp == "t") keep_N = true;
                }

                std::string line;
                uint count = 0;

                if (keep_N){

                    while(std::getline(inputFile, line)){

                        if (line[0] != '>' and line.size() > 1){

                            outputFile << '>' << std::to_string(count) << "\n" << line;
                            ++count;
                        }
                    }

                } else {

                    while(std::getline(inputFile, line)){

                        if (line[0] != '>' and line.find('N') == std::string::npos and line.size() > 1){

                            outputFile << '>' << std::to_string(count) << "\n" << line;
                            ++count;
                        }
                    }
                }

            } else {

                if (!inputFile.is_open()){

                    std::cerr << "Error : can't open the file """ << backupFilePath << """, please check your input." << std::endl;
                }

                if (!outputFile.is_open()){

                    std::cerr << "Error : can't open the file """ << inputFilePath << """, please check your input." << std::endl;
                }

                return -2;
            }
        }
    }

    return 0;
}

