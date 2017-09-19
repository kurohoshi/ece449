#ifndef LEX
#define LEX

#include <string>
#include <vector>

#include "evl_token.h"

bool extract_tokens_from_file(std::string file_name,
    std::vector<evl_token> &tokens);

#endif