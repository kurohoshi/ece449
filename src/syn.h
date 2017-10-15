#ifndef SYN
#define SYN

#include <string>
#include <list>

#include "lex.h"

class statements {

    class evl_statement {
    public:
        enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};

        statement_type type;
        evl_tokens tokens;
    }; // evl_statement class

    std::list<evl_statement> evl_statements;

public:
    bool group_tokens_into_statements(
        evl_statements &statements,
        evl_tokens &tokens
    );
}; // evl_statement class


class evl_wire {
public:
    std::string name;
    int width;
}; // evl_wire class

typedef std::list<evl_pin> evl_pins;

class evl_component {
public:
    std::string type;
    std::string name;
    evl_pins pins;
}; // evl_component class

typedef std::list<evl_component> evl_compoenents;

class evl_module {
public:
    std::string name;
    evl_wires wires;
    evl_components components;

    bool analyze_statements(
        evl_statements &statements,
        evl_modules &modules);
}; // evl_module class

typedef std::list<evl_module> evl_modules;

#endif
