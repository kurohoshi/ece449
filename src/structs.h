#ifndef EVL_TOKEN
#define EVL_TOKEN

#include <string>
#include <list>

struct evl_token {
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std::string str;

    int line_no;
}; //struct evl_token

typedef std::list<evl_token> evl_tokens;

struct evl_statement {
    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};

    statement_type type;
    evl_tokens tokens;
}; // struct evl_statement

typedef std::list<evl_statement> evl_statements;

struct evl_wire {
    std::string name;
    int width;
}; // struct evl_wire

typedef std::list<evl_wire> evl_wires;

#endif
