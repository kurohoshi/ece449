#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "evl_token.h"
#include "store_token.h"

void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == evl_token::SINGLE) {
            out << "SINGLE " << tokens[i].str << std::endl;
        } else if (tokens[i].type == evl_token::NAME) {
            out << "NAME " << tokens[i].str << std::endl;
        } else { // must be NUMBER
            out << "NUMBER " << tokens[i].str << std::endl;
        }
    }
}

bool store_tokens_to_file(std::string file_name,
    const std::vector<evl_token> &tokens) {
    std::ofstream output_file(file_name.c_str());

    if (!output_file){
        std::cerr << "I can't write " << file_name << ".tokens ." << std::endl;
        return -1;
    }

    display_tokens(output_file, tokens);
    return true;
}
