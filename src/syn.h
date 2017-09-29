#ifndef SYN
#define SYN

#include "structs.h"

bool group_tokens_into_statements(
    evl_statements &statements,
    evl_tokens &tokens
);

bool analyze_statements(
    evl_statements &statements,
    evl_modules &modules);

#endif
