#ifndef EVL_TOKEN
#define EVL_TOKEN

#include <string>
#include <list>

class evl_token {
public:
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std::string str;
    int line_no;
//public:
//    evl_token(token_type type, std::string str, int line_no);
    
}; // evl_token class

typedef std::list<evl_token> evl_tokens;

class evl_statement {
public:
    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};

    statement_type type;
    evl_tokens tokens;
//public:
//    evl_statement(statement_type type, evl_tokens tokens);
}; // evl_statement class

typedef std::list<evl_statement> evl_statements;

class evl_wire {
public:
    std::string name;
    int width;
//public:
//    evl_wire(std::string name, int width);
}; // evl_wire class

typedef std::list<evl_wire> evl_wires;

class evl_pin {
    std::string name;
    int bus_msb;
    int bus_lsb;
}; // pin class

typedef std::list<evl_pin> evl_pins;

class evl_component {
public:
    std::string type;
    std::string name;
    evl_pins pins;
}; // component class

typedef std::list<evl_component> evl_components;

class evl_module {
public:
    std::string name;
    evl_wires wires;
    evl_components components;
}; // a module class

typedef std::list<evl_module> evl_modules;

#endif
