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
    int max_width_;
public:
    net(std::string net_name, int bus)
        : name_(net_name),max_width_(bus)  {}

    void append_pin(pin *p);

    std:string get_name() { return name_; }
    void set_name(std::string new_name) { name_ = new_name; }

    char get_signal() { return signal_; }
    void set_signal(char new_signal) { signal_ = new_signal; }

    std::list<pin *> get_connections() { return connections_; }
    void add_connection(pin *new_pin) { connections_.push_back(new_pin); }
    void del_connection() { connections_.pop_back(); }


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

    char get_dir() { return dir_; }
    void set_dir(char new_dir) { dir_ = dir; }

    gate get_gate() { return gate_; }
    void set_get(gate *new_gate) { gate_ = new_gate; }

    size_t get_index() { return index_; }
    void set_index(size_t new_index) { index_ = new_index; }

    net get_net() { return net_; }
    void set_net(net *new_net) { net_ = new_net; }
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

    std::string get_name() { return name_; }
    void set_name(std::string new_name) { name_ = new_name; }

    std::string get_type() { return type_; }
    void set_type(std::string new_type) { type_ = new_type; }

    std::vector<pin *> get_pin() { return pins_; }
    void add_pin(pin *new_pin) { pins_.push_back(new_pin); }
    void pop_pin() { pins_.pop_back(); }
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
        const evl_module &module,
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
