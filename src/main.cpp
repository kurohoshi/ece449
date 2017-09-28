#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "structs.h"

#include "lex.h"
#include "syn.h"
#include "store_token.h"

int main(int argc, char *argv[]){
    //throw error if no file is provided for lex analysis
    if (argc < 2) {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }

    std::string evl_file = argv[1];
    evl_tokens tokens;
    if (!extract_tokens_from_file(evl_file, tokens)) {
        return -1;
    }
    display_tokens(std::cout, tokens); //look at the output of tokens
    if (!store_tokens_to_file(evl_file+".tokens", tokens)) {
        return -1;
    }

    evl_statements statements;
    if(!group_tokens_into_statements(statements, tokens)) {
        return -1;
    }
    display_statements(std::cout, statements); //look at the output of statements
    //if (!store_statements_to_file(evl_file+".syntax", statements)) {
    //    return -1;
    //}

    return 0;
}
