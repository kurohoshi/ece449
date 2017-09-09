#include <iostream>
#include <fstream>
#include <string>

struct tok{
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std::string str;

    int line_no;
}; //struct tok



int main(int argc, char *argv[]){
    //throw error if no file is provided for lex analysis
    if (argc < 2){
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }

    //initialize input file to be read
    //throw error if input file cannot be read
    std::ifstream input_file(argv[1]);
    if (!input_file){
        std::cerr << "I can't read" << argv[1] << "." << std::endl;
        return -1;
    }

    //name output file as input file + tokens
    std::string output_file_name = std::string(argv[1])+".tokens";
    std::ofstream output_file(output_file_name);
    //throw error if output file cannot be written
    if (!output_file){
        std::cerr << "I can't write " << argv[1] << ".tokens ." << std::endl;
        return -1;
    }

    //iterate thru file for each line starting fromn first line
    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no){
        //iterate through each character of the line
        for (size_t i = 0; i < line.size();){

            // comments
            if (char_check(line[i], "/")){
                ++i;
                if ((i == line.size()) || char_check(line[i], "/")){
                    std::cerr << "LINE " << line_no
                        << ": a single / is not allowed" << std::endl;
                    return -1;
                }
                break; // skip the rest of the line by exiting the loop
            }

            //spaces
            if (char_check(line[i], " \t\r\n")){
                ++i; // skip this space character
                continue; // skip the rest of the iteration
            }

            // SINGLE
            if (char_check(line[i], "()[]:;,")){
                output_file << "SINGLE " << line[i] << std::endl;
                ++i; // we consumed this character
                continue; // skip the rest of the iteration
            }

            // NAME
            if (range_check(line[i], 2) ||
                range_check(line[i], 3) ||
                char_check(line[i], "_")){
                size_t name_begin = i;
                for (++i; i < line.size(); ++i){
                    if (!(range_check(line[i], 1) ||
                        range_check(line[i], 2) ||
                        range_check(line[i], 3) ||
                        char_check(line[i], "_$"))){
                        break; // [name_begin, i) is the range for the token
                    }
                }
                output_file << "NAME "
                    << line.substr(name_begin, i-name_begin) << std::endl;
                continue;
            }

            //NUMBER
            if(range_check(line[i], 1)){
                size_t num_begin = i;
                for(++i; i < line.size(); ++i){
                    if(char_check(line[i], " \t\r\n()[]:;,")){
                        break; //break if whitespace/single token type encountered
                    }

                    //throw error if not whitespace, singles, nor number
                    if(!(range_check(line[i], 1))){
                        //throw error for not a number
                        std::cerr << "LINE " << line_no
                            << ": INVALID NUMBER" << std::endl;
                        return -1;
                    }
                    continue; //continue to check next character in number string
                }
                output_file << "NUMBER "
                    << line.substr(num_begin, i-num_begin) << std::endl;
                continue;
            }

            else{
                std::cerr << "LINE " << line_no
                    << ": invalid character" << std::endl;
                return -1;
            }
        }
    }

    return 0;
}
