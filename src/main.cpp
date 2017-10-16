#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "lex.h"
#include "syn.h"

int main(int argc, char *argv[]){
    //throw error if no file is provided for lex analysis
    if (argc < 2) {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }

    //std::cout << "Tokenizing File..." << std::endl;
    std::string evl_file = argv[1];
    evl_tokens tokens;
    if (!tokens.extract_file(evl_file)) {
        return -1;
    }
    //std::cout << "Tokenizing Complete..." << std::endl;
    //std::cout << "Displaying Tokens..." << std::endl;
    //display_tokens(std::cout, tokens); //look at the output of tokens
    //std::cout << "Storing Tokens into File..." << std::endl;
    if (!tokens.store(evl_file+".tokens")) {
        return -1;
    }
    //std::cout << "Storing Complete..." << std::endl;

    //std::cout << "Oganizing Statements into Modules..." << std::endl;
    evl_modules modules;
    if(!modules.group(tokens)) {
        return -1;
    }
    //std::cout << "Organizing Complete..." << std::endl;
    //std::cout << "Displaying Modules..." << std::endl;
    modules.display(std::cout); //look at the output of modules
    //std::cout << "Storing Modules into File..." << std::endl;
    if(!modules.store(evl_file+".syntax")) {
        return -1;
    }
    //std::cout << "Storing Complete" << std::endl;

    return 0;
}
