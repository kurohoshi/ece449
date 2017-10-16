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
}; // net class

class pin{
    char dir_;
    gate *gate_;
    size_t index_;
    net *net_;
}; // pin class

class gate{
    std::string name_;
    std::string type_;
    std::vector<pin *> pins_;
}; // gate class

class netlist{
    std::list<gate *> gates_;
    std::list<net *> nets_;
    std::map<std::string, net *> nets_table_;
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
