#include <iostream>
#include <fstream>
#include <string>

struct tok{
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std:string str;

    int line_no;
}; //struct tok

int char_check(char chr, const std::string& str){ //check matching char
    for(auto c: str){ //iterate thru all char of string
        if(chr == c){
            return 1; //return true if match
        }
    }
    return 0; //return false if no match
}

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
        std::cerr << "I can't read " << argv[1] << "." << std::endl;
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
            //print current character to be processed
            //std::cout << line[i] << std::endl;

            //std::cout << "checking comments" << std::endl;
            // comments
            if (line[i] == '/'){
                ++i;
                if ((i == line.size()) || (line[i] != '/')){
                    std::cerr << "LINE " << line_no
                        << ": a single / is not allowed" << std::endl;
                    return -1;
                }
                break; // skip the rest of the line by exiting the loop
            }

            //std::cout << "checking whitespaces" << std::endl;
            // spaces
            if ((line[i] == ' ') || (line[i] == '\t')
                || (line[i] == '\r') || (line[i] == '\n')){
                ++i; // skip this space character
                continue; // skip the rest of the iteration
            }

            //std::cout << "checking singles" << std::endl;
            // SINGLE
            if ((line[i] == '(') || (line[i] == ')')
                || (line[i] == '[') || (line[i] == ']')
                || (line[i] == ':') || (line[i] == ';')
                || (line[i] == ',')){
                output_file << "SINGLE " << line[i] << std::endl;
                ++i; // we consumed this character
                continue; // skip the rest of the iteration
            }

            //std::cout << "checking name" << std::endl;
            // NAME
            if (((line[i] >= 'a') && (line[i] <= 'z'))       // a to z
                || ((line[i] >= 'A') && (line[i] <= 'Z'))    // A to Z
                || (line[i] == '_')){
                size_t name_begin = i;
                for (++i; i < line.size(); ++i){
                    if (!(((line[i] >= 'a') && (line[i] <= 'z'))
                        || ((line[i] >= 'A') && (line[i] <= 'Z'))
                        || ((line[i] >= '0') && (line[i] <= '9'))
                        || (line[i] == '_') || (line[i] == '$'))){
                        break; // [name_begin, i) is the range for the token
                    }
                }
                output_file << "NAME "
                    << line.substr(name_begin, i-name_begin) << std::endl;
                continue;
            }

            //std::cout << "checking number" << std::endl;
            //NUMBER
            if((line[i] >= '0') && (line[i] <= '9')){
                size_t num_begin = i;
                for(++i; i < line.size(); ++i){
                    if((line[i] == ' ') || (line[i] == '\t')
                       || (line[i] == '\r') || (line[i] == '\n')
                       || (line[i] == '(') || (line[i] == ')')
                       || (line[i] == '[') || (line[i] == ']')
                       || (line[i] == ':') || (line[i] == ';')
                       || (line[i] == ',')){
                        break; //break if whitespace or single token type encountered
                    }

                    //throw error if not whitespace, singles, nor number
                    if(!((line[i] >= '0') && (line[i] <= '9'))){
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
