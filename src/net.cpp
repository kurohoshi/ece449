#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>

#include "net.h"

bool netlist::create_nets(const evl_wires &wires) {
    // for each wire w in wires {
    //     if(width of w == 1) {
    //         create_net(w);
    //     } else {
    //         for(int i = 0; i < width of w, ++i) {
    //             create_net(w[i]);
    //     }
    return true;
}

bool netlist::create_net(std::string net_name) {
    assert(nets_table_.find(net_name) == nets_table_.end());
    net *n = new net(net_name);
    nets_table_[net_name] = n;
    nets_.push_back(n);
}

std::string netlist::make_net_name(std::string wire_name, int i) {
    assert(i>= 0);
    std::ostringstream oss;
    oss << wire_name << "[" << i << "]";
    return oss.str();
}

bool netlist::create_gates(
    const evl_components &comps,
    const evl_wires_table &wires_table) {

    // for each component c in comps {
    //     create_gate(c, wires_table);
    // }
    return true;
}

bool netlist::create(
    const evl_wires &wires,
    const evl_components &comps,
    const evl_wires_table &wires_table) {

    return create_nets()
        && create_gates();
}

bool netlist::create_gate(
    const evl_component &c,
    const evl_wires_table &wires_table) {

    gate *g = new gate;
    gates_.push_back(g);
    return g->create(c, nets_table_, wires_table);
}

void netlist::display(
    std::ostream &out) {

    std::cout << "Display Not Implemented" << std::endl;
}

bool netlist::store(
    std::string file_name) {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display(output_file);
    return true;
}
