#ifndef TOKEN_GEN
#define TOKEN_GEN

#include <string>
#include <list>

#include "structs.h"

bool extract_tokens_from_file(
    std::string file_name,
    evl_tokens &tokens
);

#endif
