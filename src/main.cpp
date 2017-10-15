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

    //std::cout << "Tokenizing File..." << std::endl;
    std::string evl_file = argv[1];
    evl_tokens tokens;
    if (!tokens.extract(evl_file)) {
        return -1;
    }
    //std::cout << "Tokenizing Complete..." << std::endl;
    //std::cout << "Displaying Tokens..." << std::endl;
    //display_tokens(std::cout, tokens); //look at the output of tokens
    //std::cout << "Storing Tokens into File..." << std::endl;
    if (!tokens.save(evl_file+".tokens")) {
        return -1;
    }
    //std::cout << "Storing Complete..." << std::endl;

    //std::cout << "Grouping Tokens into Statements..." << std::endl;
    evl_statements statements;
    if(!group_tokens_into_statements(statements, tokens)) {
        return -1;
    }
    //std::cout << "Grouping Complete..." << std::endl;
    //std::cout << "Displaying Statements..." << std::endl;
    //display_statements(std::cout, statements);

    //std::cout << "Oganizing Statements into Modules..." << std::endl;
    evl_modules modules;
    if(!analyze_statements(statements, modules)) {
        return -1;
    }
    //std::cout << "Organizing Complete..." << std::endl;
    //std::cout << "Displaying Modules..." << std::endl;
    display_modules(std::cout, modules); //look at the output of modules
    //std::cout << "Storing Modules into File..." << std::endl;
    if(!store_modules_to_file(evl_file+".syntax", modules)) {
        return -1;
    }
    //std::cout << "Storing Complete" << std::endl;

    return 0;
}
