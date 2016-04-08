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

// Output reads with errors to easily spot differences between reads
int main(int argc, char* argv[]){

    if (argc < 6) {

        std::cerr << "Usage: get_bad_reads file1 file2 reads_file errors_sizes_file output_file" << std::endl << std::endl;

        return -1;
    }

    // Retrieve command line arguments
    std::string file1_p = argv[1];
    std::string file2_p = argv[2];
    std::string reads_p = argv[3];
    std::string errorsFile_p = argv[4];
    std::string ofile_p = argv[5];
    std::string ofile_html_p = ofile_p + ".html";

    // Initialize file streams
    std::ifstream file1;
    std::ifstream file2;
    std::ifstream reads_f;
    std::ifstream errorsFile;
    std::ofstream ofile;
    std::ofstream ofile_html;

    // Open files
    file1.open(file1_p);
    file2.open(file2_p);
    reads_f.open(reads_p);
    errorsFile.open(errorsFile_p);
    ofile.open(ofile_p);
    ofile_html.open(ofile_html_p);

    if (file1.is_open() and file2.is_open() and reads_f.is_open() and errorsFile.is_open() and ofile.is_open() and ofile_html.is_open()){

        std::string line;
        std::vector<std::string> t_line, s_line;
        std::vector<uint> errors, t_pos;
        std::vector<std::vector<uint> > positions; // Container for error positions
        std::vector<ReadPair> reads;

        // Load the position of every error into a vector
        while(std::getline(errorsFile, line)){

            /* Errors file format :
             * read number : position1, position2, ..., position n    --> reads with errors
             * size_difference_n_m    --> when reads have different sizes
             * unaligned    --> when read was not aligned
             */

            s_line = split(line, " : "); // Split read number from positions

            t_pos.resize(0);

            if(s_line.size()>0){

                errors.push_back(std::stoi(s_line[0])); // read

                // Filtering out size differences and unaligned reads, keeping only differences
                if (s_line[1].size() > 5 and (s_line[1].substr(0,4) == "size" or s_line[1].substr(0,4) == "unal")){

                    t_pos.push_back(-1);

                } else {

                    t_line = split(s_line[1], ", "); // Split different positions

                    for (uint i=0; i<t_line.size(); ++i){

                        t_pos.push_back(std::stoi(t_line[i]));
                    }

                }
                /* Result:
                * - vector containing the ID of reads with errors
                * - vector1 of vector2 of positions (each vector2 corresponds to one read)
                */
                positions.push_back(t_pos);

            }
        }

        std::cout << " - Errors file loaded (" << std::to_string(errors.size()) << " errors)." << std::endl;
        errorsFile.close();

        // Sequences from files 1 and 2 for reads with errors are stored in ReadPair object
        ReadPair tempReadPair;
        std::vector<uint>::iterator e = errors.begin();
        bool print = false;

        uint t;

        /* Store sequence for all reads with errors from file 1
         * Check for each read whether read number is in errors vector (which is sorted) using an iterator on errors vector
         * If read contains errors, iterator is incremented
         */
        while(std::getline(file1, line) and e != errors.end()){

            if (line[0] == '>'){

                t = stoi(line.substr(1));

                if (t == *e){

                    tempReadPair.id = *e;
                    print = true;
                    ++e;
                }

            } else if (print){

                tempReadPair.seq_1 = line;
                reads.push_back(tempReadPair);
                print = false;
            }

        }

        file1.close();

        std::cout << " - File 1 loaded (" << std::to_string(reads.size()) << " reads)." << std::endl;

        print = false;
        e = errors.begin();
        uint r = 0;

        // Store sequence for all reads with errors from file 2 (same as file 1)
        while (std::getline(file2, line) and e != errors.end()){

            if (line[0] == '>'){

                t = stoi(line.substr(1));

                if (t == *e){

                    print = true;
                    ++e;
                }

            } else if (print) {

                reads[r].seq_2 = line;
                ++r;
                print = false;

            }
        }

        print = false;
        e = errors.begin();
        r = 0;

        std::vector<std::vector<uint>>::iterator p = positions.begin(); // Iterator for vector of vector of errors positions
        std::vector<uint>::iterator p2; // Iterator for vector of errors positions

        // HTML stuff for formatting
        ofile_html << "<html><head><style>.all{font-family: monospace} .diff{color: red}</style></head><body><span class = ""all"">";

        /* Iterates over original reads file to retrieve original reads sequence (same as file 1 and file 2)
         * Also output results in two formats :
         *     - Plain text : id, seq_1, seq_2, seq_original
         *     - HTML : same but mismatches between sequences are colored in red (fancy!)
         */
        while (std::getline(reads_f, line) and e != errors.end()){

            if (line[0] == '>'){

                t = stoi(line.substr(1));

                if (t == *e){

                    print = true;
                    ++e;
                }

            } else if (print) {

                ofile << '>' << std::to_string(reads[r].id) << "\nRead: " << line << '\n' << file1_p << ": " << reads[r].seq_1 <<
                         '\n' << file2_p << ": " << reads[r].seq_2 << '\n';

                ofile_html << '>' << std::to_string(reads[r].id) << "<br>";

                p2 = (*p).begin(); // Reset iterator for vector of errors positions

                for (uint i=0; i<line.size();++i){

                    if (p2 != (*p).end() and i == *p2){

                        ofile_html << "<span class = ""diff"">" << line[i] << "</span>"; // class diff gives red color
                        ++p2;

                    } else {

                        ofile_html << line[i];
                    }
                }

                ofile_html << "  (Original read)<br>";

                p2 = (*p).begin(); // Reset iterator for vector of errors positions

                for (uint i=0; i<reads[r].seq_1.size();++i){

                    if (p2 != (*p).end() and i == *p2){

                        ofile_html << "<span class = ""diff"">" << reads[r].seq_1[i] << "</span>";
                        ++p2;

                    } else {

                        ofile_html << reads[r].seq_1[i];
                    }
                }

                ofile_html << "  (" << file1_p << ")<br>";

                p2 = (*p).begin(); // Reset iterator for vector of errors positions

                for (uint i=0; i<reads[r].seq_2.size();++i){

                    if (p2 != (*p).end() and i == *p2){

                        ofile_html << "<span class = ""diff"">" << reads[r].seq_2[i] << "</span>";
                        ++p2;

                    } else {

                        ofile_html << reads[r].seq_2[i];
                    }
                }

                ofile_html << "  (" << file2_p << ")<br>";

                ++r; // Increment read counter
                ++p; // Increment iterator for vector of vector of error positions
                print = false;

                ofile_html << "<br>";

            }
        }

        ofile_html << "</span> </body> </html>"; // End of HTML file

    } else {

        if (!file1.is_open()){

            std::cerr << "Error : can't open the file """ << file1_p << """, please check your input." << std::endl;
        }

        if (!file2.is_open()){

            std::cerr << "Error : can't open the file """ << file2_p << """, please check your input." << std::endl;
        }

        if (!reads_f.is_open()){

            std::cerr << "Error : can't open the file """ << reads_p << """, please check your input." << std::endl;
        }

        if (!errorsFile.is_open()){

            std::cerr << "Error : can't open the file """ << errorsFile_p << """, please check your input." << std::endl;
        }

        if (!ofile.is_open()){

            std::cerr << "Error : can't open the file """ << ofile_p << """, please check your input." << std::endl;
        }

        if (!ofile_html.is_open()){

            std::cerr << "Error : can't open the file """ << ofile_html_p << """, please check your input." << std::endl;
        }

        return -2;
    }

    return 0;

}
