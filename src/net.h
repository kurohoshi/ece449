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
} // gate class

class netlist{
    std::list<gate *> gates_;
    std::list<net *> nets_;
}; // netlist class

#endif
