#ifndef STORE_TOKENS
#define STORE_TOKENS

#include <iostream>
#include <string>
#include <vector>

#include "evl_token.h"

void display_tokens(std::ostream &out,
    const std::vector<evl_token> &tokens);
    
bool store_tokens_to_file(std::string file_name,
    const std::vector<evl_token> &tokens);

#endif
