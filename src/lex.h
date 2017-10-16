#ifndef LEX
#define LEX

#include <string>
#include <list>
#include <iostream>

struct evl_token {
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std::string str;
    int line_no;
}; // evl_token class

class tokens {
    std::list<evl_token> evl_tokens;

    bool extract_from_line(
        std::string line,
        int line_no
    );
public:
    bool extract_file(
        std::string file_name
    );

    void display(
        std::ostream &out
    ) const;

    bool store(
        std::string file_name
    ) const;
}; // evl_tokens class

#endif
