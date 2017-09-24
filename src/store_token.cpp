#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>

#include "structs.h"
#include "store_token.h"

void display_tokens(
    std::ostream &out,
    const evl_tokens &tokens) {

    for (evl_tokens::iterator i = tokens.begin(); i != tokens.end(); ++i) {
        if (*i == evl_token::SINGLE) {
            out << "SINGLE " << *i << std::endl;
        } else if (*i.type == evl_token::NAME) {
            out << "NAME " << *i << std::endl;
        } else { // must be NUMBER
            out << "NUMBER " << *i << std::endl;
        }
    }
}

void display_statements(
    std::ostream &out,
    const evl_statements &statements) {

}

bool store_tokens_to_file(
    std::string file_name,
    const evl_tokens &tokens) {

    std::ofstream output_file(file_name.c_str());

    if (!output_file){
        std::cerr << "I can't write " << file_name << ".tokens ." << std::endl;
        return -1;
    }

    display_tokens(output_file, tokens);
    return true;
}
