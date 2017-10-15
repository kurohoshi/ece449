#ifndef EVL_NET
#define EVL_NET

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
}; // net class

class pin{
    char dir_;
    gate *gate_;
    size_t index_;
    net *net_;
public:
}; // pin class

class gate{
    std::string name_;
    std::string type_;
    std::vector<pin *> pins_;
public:
} // gate class

class netlist{
    std::list<gate *> gates_;
    std::list<net *> nets_;
    std::map<std::string, net *> nets_table_;
public:
    bool create(
        const evl_wires &wires,
        const evl_components &comps,
        const evl_wires_table &wires_table);
}; // netlist class

#endif
