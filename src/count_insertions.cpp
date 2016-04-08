#include <regex>
#include <bitset>
#include "utils.h"


// Parser for cigar string. Output = array [n_insertions, n_deletions]
void cigarParser(std::string& cigar, uint* output){

    std::regex insertion_match ("(\\d+)I"); // Pattern for insertion
    std::regex deletion_match ("(\\d+)D"); // Pattern for deletion
    std::smatch matches;

    std::string cigar_copy = cigar; // Need a copy of cigar string because string is modified in regex search

    // Search for regex pattern indicating insertion / deletion in CIGAR string
    while (std::regex_search(cigar_copy, matches, insertion_match)) {

        ++output[0]; // If insertion pattern found, increment insertion counter
        cigar_copy = matches.suffix().str(); // Truncate cigar string to start after 1st match and reiterate search
    }

    cigar_copy = cigar; // Reset cigar string to original form

    while (std::regex_search(cigar_copy, matches, deletion_match)) {

        ++output[1];
        cigar_copy = matches.suffix().str();
    }
}




// Count number of insertions / deletions per read from an alignment output file (SAM)
int main(int argc, char *argv[]) {

    if (argc < 3){

        std::cerr << "Usage : ./count_insertions input_file output_file" << std::endl << std::endl;

        return -1;

    } else {

        // Retrieve command-line arguments
        std::string inputFilePath = argv[1];
        std::string outputFilePath = argv[2];

        // Initialize file streams
        std::ifstream inputFile;
        std::ofstream outputFile;

        // Open input and output files
        inputFile.open(inputFilePath);
        outputFile.open(outputFilePath);


        if (inputFile.is_open() and outputFile.is_open()){

            std::string line;
            std::vector<std::string> samLine;
            uint indels[2]; // Counter for insertions and deletions : array of size 2

            /* SAM flag is an int that can be converted to a bitset to obtain several informations about alignment.
             * Information used : read was aligned / not aligned, read was aligned on forward / reverse strand.
             * SAM flag parsing is implement in utils.h, in functions is_mapped and is_RC
             */
            std::bitset<12> flag;
            uint readSize = 0;
            std::string perfectCigar = "";

            while(std::getline(inputFile, line)){

                samLine = split(line, "\t");
                flag = std::bitset<12>(std::stoi(samLine[1])); // Retrieve alignment information as a bitset

                if (is_mapped(flag)){

                    // Initialize counters
                    indels[0] = 0;
                    indels[1] = 0;

                    /* Cigar string = 6th field in SAM format.
                     * For a read of size X, a cigar string of "XM" means no indels
                     * We can thus easily filter out reads with 0 indels
                     */
                    readSize = samLine[9].size();
                    perfectCigar = std::to_string(readSize) + "M";

                    if (samLine[5] != perfectCigar){

                        cigarParser(samLine[5], indels);
                    }

                    if (indels[0] != 0 or indels[1] != 0){

                        outputFile << samLine[0] << ", " << indels[0] << ", " << indels[1] << "\n";
                    }
                }

            }

            inputFile.close();
            outputFile.close();

        } else {

            if (!inputFile.is_open()){

                std::cerr << "Error : can't open the file """ << inputFilePath << """, please check your input." << std::endl;
            }

            if (!outputFile.is_open()){

                std::cerr << "Error : can't open the file """ << outputFilePath << """, please check your input." << std::endl;
            }

            return -2;
        }
    }

}
