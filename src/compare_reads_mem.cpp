#include "read.h"
#include "utils.h"
#include <dirent.h>


void indexFiles(std::ifstream& file1, std::ifstream& file2){

    std::cout << " - Indexing reads file ..." << std::endl;

    uint nTempFiles = 10;

    std::string fileName;
    std::vector<std::ofstream> tempFiles1, tempFiles2;

    for (uint i=0; i<nTempFiles; ++i){

        fileName = "temp_f1_" + std::to_string(i);
        std::ofstream o1, o2;
        o1.open(fileName);
        tempFiles1.push_back(move(o1));
        fileName = "temp_f2_" + std::to_string(i);
        o2.open(fileName);
        tempFiles2.push_back(move(o2));
    }

    resetFileIndex(file1);
    resetFileIndex(file2);

    std::string line;
    uint readNumber = 0, fileNumber = 0;

    while(std::getline(file1, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));
            fileNumber = readNumber % nTempFiles;
            tempFiles1[fileNumber] << line << "\n";

        } else {

            tempFiles1[fileNumber] << line << "\n";
        }
    }

    while(std::getline(file2, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));
            fileNumber = readNumber % nTempFiles;
            tempFiles2[fileNumber] << line << "\n";

        } else {

            tempFiles2[fileNumber] << line << "\n";
        }
    }

    for (uint i=0; i<nTempFiles; ++i){

        tempFiles2[i].close();
        tempFiles1[i].close();
    }

    file1.close();
    file2.close();

    std::cout << " - Indexing completed." << std::endl;
}




void processTempFiles(uint n, std::ofstream& outputFile, std::ofstream& outputFileCount){

    std::cout << " - Processing file : " << std::to_string(n) << std::endl;

    std::ifstream tempFile1, tempFile2;
    std::string fileName = "temp_f1_" + std::to_string(n);
    tempFile1.open(fileName);
    fileName = "temp_f2_" + std::to_string(n);
    tempFile2.open(fileName);

    // Find max read number in each file
    uint m = std::max(find_max(tempFile1), find_max(tempFile2));

    std::vector<ReadPair> reads;
    reads.resize(m+1);

    std::string line;
    int readNumber = -1;

    std::cout << " - Loading temp file 1" << std::endl;

    // Fill ReadPair seq_1.
    while(std::getline(tempFile1, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else {

            reads[readNumber].id = readNumber;
            reads[readNumber].seq_1 += line;
        }
    }
	
    std::cout << " - Loading temp file 2" << std::endl;

    readNumber = -1;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)
    while(std::getline(tempFile2, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else {

            if (readNumber >=0){
                reads[readNumber].seq_2 += line;
                reads[readNumber].id = readNumber;
            }

            readNumber = -1;
        }
    }

	std::cout << " - Comparing reads " << std::endl;

    // Compare reads from each ReadPair and store results in output files
    for (uint i=0; i<reads.size(); ++i){

        reads[i].compare(outputFile, outputFileCount);
    }

    tempFile1.close();
    tempFile2.close();
}




// Cleanup all the temporary files at the end of the program (all temporary files start with 'temp')
void cleanupTempFiles(){

    // Using standard libray, a directory and its content are represented by pointers with specific types
    DIR* directory =  opendir("./");
    struct dirent* directoryContent;

    if(!directory) return; // Directory not open

    // Iterates over directory content and remove each file starting with 'temp'
    while ((directoryContent=readdir(directory))){

        std::string fileName = directoryContent->d_name;

        if(fileName.find("temp") != std::string::npos){
          remove(fileName.c_str());
        }
    }

    closedir(directory);
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

        if (file1.is_open() and file2.is_open()){

            indexFiles(file1, file2);

            outputFile.open(outputFileName.c_str());
            outputFileCount.open(outputFileNameCount.c_str());

            for (uint i=0; i<10; ++i){

                processTempFiles(i, outputFile, outputFileCount);
            }

            outputFile.close();
            outputFileCount.close();

            cleanupTempFiles();

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
