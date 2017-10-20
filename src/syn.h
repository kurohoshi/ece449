#ifndef SYN
#define SYN

#include <iostream>
#include <string>
#include <list>
#include <map>

class evl_tokens;

class evl_wire {
    std::string name;
    int width;
public:
    evl_wire(std::string n, int w)
        : name(n), width(w) {}

    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }

    int get_width() const { return width; }
    void set_width(int new_width) { width = new_width; }

    ~evl_wire() {}
}; // evl_wire class

typedef std::list<evl_wire> evl_wires;

typedef std::map<std::string, int> evl_wires_table;

bool make_wires_table(
    const evl_wires &wires,
    evl_wires_table &wires_table
);

class evl_pin {
    std::string name;
    int bus_msb;
    int bus_lsb;
public:
    evl_pin(std::string n, int m, int l)
        : name(n), bus_msb(m), bus_lsb(l) {}

    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }

    int get_msb() const { return bus_msb; }
    void set_msb(int new_msb) { bus_msb = new_msb; }

    int get_lsb() const { return bus_lsb; }
    void set_lsb(int new_lsb) { bus_lsb = new_lsb; }

    ~evl_pin() {}
}; // pin class

typedef std::list<evl_pin> evl_pins;

class evl_component {
    std::string type;
    std::string name;
    evl_pins pins;
public:
    evl_component(std::string t, std::string n)
        : type(t), name(n) {}

    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }

    std::string get_type() const { return type; }
    void set_type(std::string new_type) { name = new_type; }

    void display(
        std::ostream &out
    ) const;

    evl_pins::const_iterator pins_begin() const { return pins.begin(); }
    evl_pins::const_iterator pins_end() const { return pins.end(); }
    void add_pin(evl_pin pin) { pins.push_back(pin); }

    ~evl_component() {}
}; // evl_component class

typedef std::list<evl_component> evl_components;

struct evl_module {
    std::string name;
    evl_wires wires;
    evl_components components;
public:
    evl_module() {}

    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }

    evl_wires get_wires_list() const { return wires; }

    evl_wires::const_iterator wires_begin() { return wires.begin(); }
    evl_wires::const_iterator wires_end() { return wires.end(); }

    evl_components::const_iterator components_begin() { return components.begin(); }
    evl_components::const_iterator components_end() { return components.end(); }

    bool get_module_name(
        evl_tokens &t
    );

    bool get_wires(
        evl_tokens &t
    );

    bool get_component(
        evl_tokens &t
    );

    void display(
        std::ostream &out
    ) const;

    ~evl_module() {}
}; // evl_module class

class evl_modules{
    typedef std::list<evl_module> evl_modules_;
    evl_modules_ modules;
public:
    evl_modules() {}

    evl_module front() { return modules.front(); }

    bool group(
        evl_tokens &tokens
    );

    void display(
        std::ostream &out
    ) const;

    bool store(
        std::string file_name
    ) const;

    ~evl_modules() {}
};

#endif
