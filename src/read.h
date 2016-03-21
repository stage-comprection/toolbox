#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>


struct Read{

    int id;
    std::string seq;

    bool operator < (const Read& read) const{
        return(id < read.id);
    }

    Read();

};


struct ReadPair{

    int id;
    std::string seq_1;
    std::string seq_2;

    bool operator < (const ReadPair& readPair) const{
        return(id < readPair.id);
    }

    void compare(std::ofstream& outputFile, std::ofstream& outputFileCount);
    ReadPair();

};
