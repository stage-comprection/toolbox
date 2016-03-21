#include "read.h"


Read::Read(){

    this->id = 0;
    this->seq = "";
}




ReadPair::ReadPair(){

    this->id = 0;
    this->seq_1 = "";
    this->seq_2 = "";
}




// Compares two reads and returns the positions of difference between thoses reads
void ReadPair::compare(std::ofstream& outputFile, std::ofstream& outputFileCount){

    // In case something was removed during correction or the reference is smaller/bigger...

    if (this->seq_1 != "not_aligned" and this->seq_2 != "not_aligned"){

        if (this->seq_1.size() != this->seq_2.size()){

            outputFile << std::to_string(this->id) << " : " << "size_difference_" << std::to_string(this->seq_1.size()) << "_" <<
                          std::to_string(this->seq_2.size()) << "\n";

            outputFileCount << std::to_string(this->id) << " : " << "size_difference_" << std::to_string(this->seq_1.size()) << "_" <<
                          std::to_string(this->seq_2.size()) << "\n";

        } else {

            std::vector<uint> errors;

            for (uint i=0; i<this->seq_1.size(); ++i){

                if (this->seq_1[i] != this->seq_2[i]){

                    errors.push_back(i);
                }

            }

            if (errors.size() > 0){

                outputFileCount << std::to_string(this->id)<< " : " << std::to_string(errors.size()) << "\n";
                outputFile << std::to_string(this->id)<< " : ";

                for (uint i=0; i < errors.size() - 1; ++i){

                    outputFile << std::to_string(errors[i]) << ", ";
                }

                outputFile << std::to_string(errors[errors.size()-1]) << "\n";
            }

        }

    } else {

        if (this->seq_1 == "not_aligned"){

            if (this->seq_2 == "not_aligned"){

                outputFile << std::to_string(this->id) << " : " << "seq_1_2_not_aligned\n";
                outputFileCount << std::to_string(this->id) << " : " << "seq_1_2_not_aligned\n";

            } else {

                outputFile << std::to_string(this->id) << " : " << "seq_1_not_aligned\n";
                outputFileCount << std::to_string(this->id) << " : " << "seq_1_2_not_aligned\n";

            }

        } else {

            outputFile << std::to_string(this->id) << " : " << "seq_2_not_aligned\n";
            outputFileCount << std::to_string(this->id) << " : " << "seq_1_2_not_aligned\n";

        }

    }
}

