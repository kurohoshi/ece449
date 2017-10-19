#ifndef EVL_NET
#define EVL_NET

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "syn.h"

class evl_modules;

// forward declarations
class netlist;
class net;
class pin;
class gate;

class net{
    std::string name_;
    char signal_;
    std::list<pin *> connections_;
public:
    net(std::string net_name)
        : name_(net_name) {}

    void append_pin(pin *p);
}; // net class

class pin{
    char dir_;
    gate *gate_;
    size_t index_;
    net *net_;

public:
    bool create(
        gate *g,
        size_t index,
        const evl_pin &p,
        const std::map<std::string, net *> &nets_table);
}; // pin class

class gate{
    std::string name_;
    std::string type_;
    std::vector<pin *> pins_;

    bool create_pin(
        const evl_pin &ep,
        size_t index,
        const std::map<std::string, net *> &nets_table,
        const evl_wires_table &wires_table);

public:
    bool create(
        const evl_component &c,
        const std::map<std::string, net *> &nets_table,
        const evl_wires_table &wires_table);
}; // gate class

class netlist{
    std::string name_;
    std::list<gate *> gates_;
    std::list<net *> nets_;
    std::map<std::string, net *> nets_table_;

    void create_net(
        std::string net_name);

public:
    bool create(
        const evl_wires &wires,
        const evl_components &comps,
        const evl_wires_table &wires_table
    );

    void display(
        std::ostream &out
    ) const;

    bool store(
        std::string file_name
    ) const;
}; // netlist class

#endif
