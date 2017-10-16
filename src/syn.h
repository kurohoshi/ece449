#ifndef SYN
#define SYN

#include <string>
#include <list>

#include "lex.h"

struct evl_wire {
public:
    std::string name;
    int width;
}; // evl_wire class

typedef std::list<evl_wire> evl_wires;

struct evl_pin {
public:
    std::string name;
    int bus_msb;
    int bus_lsb;
}; // pin class

typedef std::list<evl_pin> evl_pins;

struct evl_component {
public:
    std::string type;
    std::string name;
    evl_pins pins;
}; // evl_component class

typedef std::list<evl_component> evl_components;

struct evl_module {
public:
    std::string name;
    evl_wires wires;
    evl_components components;
}; // evl_module class

class modules{
    std::list<evl_module> evl_modules;

    bool get_module_name(
        std::string &name,
        evl_tokens &t
    );

    bool get_wires(
        evl_wires &wires,
        evl_tokens &t
    );

    bool get_component(
        evl_components &components,
        evl_tokens &t
    );
public:
    bool group(
        evl_statements &statements
    );

    void display(
        std::ostream &out
    ) const;

    bool store(
        std::string file_name
    ) const;
};

#endif
