#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>


using namespace std;

void formatReadsFile(string& readsFilePath, bool keepN){

    ifstream readsFile;
    string originalreadsFile = readsFilePath + ".backup";
    readsFile.open(originalreadsFile.c_str());

    ofstream formattedReadsFile;
    formattedReadsFile.open(readsFilePath.c_str());

    string line, name, sequence;
    uint count = 0;

    while(getline(readsFile, line)){

        if(line[0] == '>'){

            if (sequence.size() > 0){

                if (sequence.find("to_thrash") == string::npos){

                    formattedReadsFile<<">"<<name<<"\n"<<sequence<<"\n";

                } else {

                    --count;
                }
            }

            name = to_string(count);
            sequence = "";

            ++count;

        } else if (line.find("N") == string::npos){

            sequence += line;

        } else {

            if (keepN){

                sequence += line;

            } else {

                sequence = "to_thrash";
            }


        }
    }

    if (sequence.size() > 0){

        if (sequence.find("to_thrash") == string::npos){

            formattedReadsFile<<">"<<name<<"\n"<<sequence<<"\n";

        } else {

            --count;
        }
    }

    readsFile.close();
    formattedReadsFile.close();
}



int main(int argc, char *argv[]){

    if (argc < 3){
        cout<<"\nError : not enough arguments, exiting...\n";
        return -1;
    }

    string filePath = argv[1];
    string keepN_s = argv[2];
    bool keepN = true;
    if (keepN_s != "T") keepN = false;
    string readsFilePath = filePath;
    formatReadsFile(readsFilePath, keepN);
    return 0;
}

