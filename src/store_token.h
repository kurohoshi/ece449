#ifndef STORE_TOKENS
#define STORE_TOKENS

#include <iostream>
#include <string>
#include <list>

#include "structs.h"

void display_tokens(
    std::ostream &out,
    const evl_tokens &tokens);

void display_modules(
    std::ostream &out,
    const evl_modules &modules);

bool store_tokens_to_file(
    std::string file_name,
    const evl_tokens &tokens);

bool store_modules_to_file(
    std::string file_name,
    const evl_modules &modules);
#endif
