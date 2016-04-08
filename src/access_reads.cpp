#include "utils.h"
#include "read.h"
#include <sstream>


/*
 * For sorted reads fasta file / SAM file
 */

// Checks whether a file is in SAM format (if the file is not FASTA, it's SAM)
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




// Load reads file in memory and quickly access a read via its ID
int main(int argc, char *argv[]) {

    if (argc < 2){

        std::cerr << "Usage : ./access_reads file\n\n" << std::endl << std::endl;

        return -1;

    } else {

        // Open file from arguments
        std::string fileName = argv[1];
        std::ifstream file;
        file.open(fileName.c_str());

        if (file.is_open()){

            // Check if file is SAM format
            bool sam = is_sam(file);

            uint fileSize = 0;
            std::string line;

            // Count number of reads in file
            if (!sam){

                fileSize = countReads(file);

            } else {

                while(getline(file, line)){

                    if (line[0] != '@'){

                        ++fileSize;
                    }
                }

                resetFileIndex(file); // Reset stream pointer to beginning of the file
            }

            std::cout << " - Reading file (";

            if(sam) std::cout << "Format : SAM) ...\n"<< std::endl; else std::cout << "Format : not SAM) ...\n" << std::endl;

            // Initialize an empty vector of reads of size = number of reads
            std::vector<Read> reads;
            reads.resize(fileSize);

            // Assign an ID to each read of the vector
            for (uint i=0; i<reads.size(); ++i){
                reads[i].id = i;
            }

            uint readNumber = 0;

            // Fill vector of reads (according to file format)
            if (sam){

                std::vector<std::string> s_line;

                while(std::getline(file, line)){

                    s_line = split(line, "\t");
                    reads[std::stoi(s_line[0])].seq = line; // For SAM, the entire line is stored as read sequence
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

            sort(reads.begin(), reads.end()); // Sort reads according to their ID

            std::cout << " - File loaded\n\n" << std::endl;

            int iss_num = 0; // Variable to store input as an int
            std::string input = "exit";

            do {
                // Retrieve input and convert it to isstringstream (to easily convert it to an int later)
                std::cout << "Enter read number : ";
                std::cin >> input;
                std::istringstream iss(input);

                if (!(iss >> iss_num).fail()) { // Check whether input is an int

                    std::cout << ">" << reads[iss_num].id << "\n" << reads[iss_num].seq << "\n\n";

                } else if (input != "exit" and input != "quit"){ // Inputting "exit" or "quit" closes the program

                    std::cout << "You did not enter a valid line number. To leave the program, type 'exit' or 'quit'.\n" << std::endl;
                }

            } while (input != "exit" and input != "quit");

        } else {

            std::cerr << "Error : can't open the file """ << fileName << """, please check your input." << std::endl;

            return -2;
        }
    }

    return 0;

}
