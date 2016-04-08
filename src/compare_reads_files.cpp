#include "read.h"
#include "utils.h"


/* Store two fasta files as a ReadPair object (Properties : ID, seq_1, seq_2)
 * Input : 2 file streams, 1 vector of ReadPairs of size = number of reads
 */
void loadReadsFromFiles(std::ifstream& file1, std::ifstream& file2, std::vector<ReadPair>& reads){

    std::string line;
    uint readNumber = 0;

    // Fill ReadPair seq_1.
    while(std::getline(file1, line)){

        if (line[0] == '>'){

            readNumber = std::stoi(line.substr(1));

        } else {

            reads[readNumber].seq_1 += line;
        }
    }

    std::cout << " - File 1 loaded" << std::endl;

    // Sort ReadPair vector according to ID
    std::sort(reads.begin(), reads.end());

    std::cout << " - File 1 sorted" << std::endl;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)
    while(std::getline(file2, line)){

        if (line[0] == '>'){

            readNumber = std::stoi(line.substr(1));

        } else {

            reads[readNumber].seq_2 += line;
        }
    }

    std::cout << " - File 2 loaded" << std::endl;

}




// Compare two reads file and store differences in output file
int main(int argc, char *argv[]) {

    if (argc < 4){

        std::cerr << "Usage : ./compare_reads_files file1 file2 output" << std::endl << std::endl;

        return -1;

    } else {

        // Retrieve command-line arguments
        std::string file1Name = argv[1];
        std::string file2Name = argv[2];
        std::string outputFileName = argv[3];
        std::string outputFileNameCount = outputFileName + "_sizes";

        // Initialize file streams
        std::ifstream file1;
        std::ifstream file2;
        std::ofstream outputFile;
        std::ofstream outputFileCount;

        // Open input files
        file1.open(file1Name.c_str());
        file2.open(file2Name.c_str());

        // Count number of reads in each file
        uint file1_n = countReads(file1);
        uint file2_n = countReads(file2);

        if (file1_n != file2_n){

            std::cerr << "Error : input files have different number of reads. Please check your input." << std::endl;
            std::cerr << "File 1 : " << std::to_string(file1_n) << " reads." << std::endl;
            std::cerr << "File 2 : " << std::to_string(file2_n) << " reads." << std::endl;
            return -2;
        }

        if (file1.is_open() and file2.is_open()){

            // Initialize an empty vector of ReadPairs of size = number of reads
            std::vector<ReadPair> reads;
            reads.resize(file1_n);

            // Initialize ReadPairs IDs ( id = position of the read : first read = 0, second read = 1 ...)
            for (uint i=0; i<reads.size();++i){
                reads[i].id = i;
            }

            // Fill ReadPair sequences (seq_1, seq_2)
            loadReadsFromFiles(file1, file2, reads);

            outputFile.open(outputFileName.c_str());
            outputFileCount.open(outputFileNameCount.c_str());

            // Compare reads from each ReadPair and store results in output files
            for (uint i=0; i<reads.size(); ++i){

                reads[i].compare(outputFile, outputFileCount);
            }

            outputFile.close();
            outputFileCount.close();

        } else {

            if (!file1.is_open()){

                std::cerr << "Error : can't open the file """ << file1Name << """, please check your input." << std::endl;
            }

            if (!file2.is_open()){

                std::cerr << "Error : can't open the file """ << file2Name << """, please check your input." << std::endl;
            }

            return -2;
        }
    }

    return 0;

}
