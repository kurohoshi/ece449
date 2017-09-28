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

    for(evl_tokens::const_iterator token = tokens.begin(); token != tokens.end(); ++token) {
        switch(token->type) {
            case evl_token::SINGLE:
                out << "SINGLE " << token->str << std::endl;
                break;
            case evl_token::NAME:
                out << "NAME " << token->str << std::endl;
                break;
            case evl_token::NUMBER:
                out << "NUMBER " << token->str << std::endl;
                break;
            default:
                std::cerr << "Something broke..." << std::endl;
                break;
        }
    }
}

void display_statements(
    std::ostream &out,
    const evl_statements &statements) {

    for(evl_statements::const_iterator statement = statements.begin(); statement != statements.end(); ++statement) {
        for(evl_tokens::const_iterator token = statement->tokens.begin(); token != statement->tokens.end(); ++token) {
            out << token->str;
        }
        out << std::endl;
/*
        switch(statement->type) {
            case evl_statement::MODULE:
                break;
            case evl_statement::WIRE:
                break;
            case evl_statement::COMPONENT:
                break;
            case evl_statement::ENDMODULE:
                break;

        }
*/
    }
}

bool store_tokens_to_file(
    std::string file_name,
    const evl_tokens &tokens) {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display_tokens(output_file, tokens);
    return true;
}

bool store_statements_to_file(
    std::string file_name,
    const evl_statements &statements) {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display_statements(output_file, statements);
    return true;
}
