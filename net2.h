#ifndef NET
#define NET

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
        : name_(net_name)  {}

    void append_pin(pin *p);

    std::string get_name() const { return name_; }

    char get_signal() const { return signal_; }
    void set_signal(char new_signal) { signal_ = new_signal; }

    int connections_size() const { return connections_.size(); }
    std::list<pin *>::const_iterator connections_begin()
        { return connections_.begin(); }
    std::list<pin *>::const_iterator connections_end()
        { return connections_.end(); }

    bool validate_connections();

    void display(
        std::ostream &out
    ) const;

    // how to access pins_ vector
}; // net class

class gate{
    gate(const gate &) = delete;            // no copy
    gate &operator=(const gate &) = delete; // no assignment
    std::string name_;
    std::string type_;
    std::vector<pin *> pins_;

    bool validate_structural_semantics();
public:
    bool create(
        const evl_component &c,
        const std::map<std::string, net *> &nets_table,
        const evl_module::evl_wires_table &wires_table);

    std::string get_name() const { return name_; }
    std::string get_type() const { return type_; }

    void display(
        std::ostream &out
    ) const;
}; // gate class

class pin{
    char dir_;
    gate *gate_;
    size_t index_;
    std::list<net *> nets_;
public:
    pin() {}

    bool create(
        gate *g,
        size_t index,
        const evl_pin &p,
        const std::map<std::string, net *> &nets_table);

    char get_dir() const { return dir_; }
    void set_dir(char c) { dir_ = c; }
    gate* get_gate() const { return gate_; }
    size_t get_index() const { return index_; }

    void display(
        std::ostream &out
    ) const;
}; // pin class

class netlist{
    std::string name_;
    std::list<gate *> gates_;
    std::list<net *> nets_;
    std::map<std::string, net *> nets_table_;

    void create_net(
        std::string net_name);

public:
    bool create(
        const evl_module &module
    );

    void display(
        std::ostream &out
    ) const;

    bool store(
        std::string file_name
    ) const;
}; // netlist class

//********************************************************
//             Inheritance (Flip_Flop)
//********************************************************
class flip_flop: public gate {
    char state_, next_state_;
public:
    bool validate_structural_semantics();
    void compute_next_state_or_output();
    char compute_signal(int pin_index);
}; // class flip_flop

bool flip_flop::validate_structural_semantics() {
    if(pins_.size() != 3)
        return false;
    pins_[0]->set_as_output(); // q
    pins_[1]->set_as_input(); // d
    pins_[2]->set_as_input(); // clk
}

void flip_flop::compute_next_state_or_output() {
    next_state_ = pins_[1]->compute_signal(); // d
}

char flip_flop::compute_signal(int pin_index) {
    assert pin_index == 0; // must be q
    return state_;
}

#endif
