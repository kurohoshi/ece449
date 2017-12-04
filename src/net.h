#ifndef NET
#define NET

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

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

    char get_signal();

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

    gate(const gate &) = delete;
    gate &operator=(const gate &) = delete;

protected:
    std::string name_;
    std::string type_;
    std::vector<pin *> pins_;

    virtual bool validate_structural_semantics();

public:
    gate(std::string type, std::string name)
        : type_(type), name_(name) {}
    gate() {}

    bool create(
        const evl_component &c,
        const std::map<std::string, net *> &nets_table,
        const evl_module::evl_wires_table &wires_table);

    virtual void compute_state_or_output(std::string file_name);
    virtual char compute_signal(int pin_index);

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

    char compute_signal();
    std::string compute_bus_signal();

    char get_dir() const { return dir_; }
    void set_as_input() { dir_ = 'I'; }
    void set_as_output() { dir_ = 'O'; }
    gate* get_gate() const { return gate_; }
    size_t get_index() const { return index_; }
    size_t nets_size() const { return nets_.size(); }

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

    void compute_state_and_outputs(std:string file_name);
}; // netlist class

#endif
