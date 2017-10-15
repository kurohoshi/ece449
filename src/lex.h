#ifndef LEX
#define LEX

#include <string>
#include <list>

#include "structs.h"


class tokens {

    class evl_token {
    public:
        enum token_type {NAME, NUMBER, SINGLE};

        token_type type;
        std::string str;
        int line_no;
    }; // evl_token class

    std::list<evl_token> evl_tokens;

public:

    bool extract(
        std::string file_name
    );
}; // evl_tokens class


#endif
