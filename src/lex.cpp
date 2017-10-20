#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>

#include "lex.h"

bool evl_tokens::extract_from_line(
    std::string line,
    int line_no) {

    char SPACE_CHAR[] = " \t\r\n";
    char SINGLE_CHAR[] = "()[]:;,";

    for (size_t i = 0; i < line.size();) {
        if (line[i] == '/') { //comments`
            ++i;
            if ((i == line.size()) || (line[i] != '/')) {
                std::cerr << "LINE " << line_no
                    << ": a single / is not allowed" << std::endl;
                return false;
            }
            break; // skip the rest of the line by exiting the loop
        } else if (strchr(SPACE_CHAR, line[i])) { //spaces
            ++i; // skip this space character
            continue;
        } else if (strchr(SINGLE_CHAR, line[i])) { //SINGLES
            evl_token *token = new evl_token(
                evl_token::SINGLE,
                std::string(1, line[i]),
                line_no);
            tokens.push_back(*token);
            ++i;
            continue;
        } else if (isalpha(line[i]) || (line[i] == '_')) {
            size_t name_begin = i;
            for (++i; i < line.size(); ++i) {
                if (!(isalpha(line[i]) || isdigit(line[i]) ||
                    (line[i] == '_') || (line[i] == '$'))) {
                    break; // [name_begin, i) is the range for the token
                }
            }
            evl_token *token = new token(
                evl_token::NAME,
                line.substr(name_begin, i-name_begin),
                line_no);
            tokens.push_back(*token);
            continue;
        } else if(isdigit(line[i])) { //NUMBER
            size_t num_begin = i;
            for(++i; i < line.size(); ++i) {
                if(strchr(SPACE_CHAR, line[i]) || strchr(SINGLE_CHAR, line[i])){
                    break; //break if whitespace/single token type encountered
                }
                if(!(isdigit(line[i]))) {
                    //throw error for not a number
                    std::cerr << "LINE " << line_no
                        << ": INVALID NUMBER" << std::endl;
                    return false;
                }
                continue; //continue to check next character in number string
            }
            evl_token *token = new evl_token(
                evl_token::NUMBER,
                line.substr(num_begin, i-num_begin),
                line_no);
            tokens.push_back(*token);
            continue;
        } else { //INVALID TOKEN
            std::cerr << "LINE " << line_no
                << ": invalid character" << std::endl;
            return false;
        }
    }
    return true; // nothing left
}

bool evl_tokens::extract_file(
    std::string file_name) {

    std::ifstream input_file(file_name);
    if (!input_file) {
        std::cerr << "I can’t read " << file_name << "." << std::endl;
        return false;
    }
    tokens.clear(); // be defensive, make it empty
    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no) {
        if (!extract_from_line(line, line_no)) {
          return false;
        }
    }
    return true;
}

void evl_tokens::display(
    std::ostream &out) const {

    for(evl_tokens_::const_iterator token = tokens.begin();
        token != tokens.end(); ++token) {

        switch(token->get_type()) {
            case evl_token::SINGLE:
                out << "SINGLE " << token->get_str() << std::endl;
                break;
            case evl_token::NAME:
                out << "NAME " << token->get_str() << std::endl;
                break;
            case evl_token::NUMBER:
                out << "NUMBER " << token->get_str() << std::endl;
                break;
            default:
                std::cerr << "Something broke..." << std::endl;
                break;
        }
    }
}

bool evl_tokens::store(
    std::string file_name) const {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display(output_file);
    return true;
}
