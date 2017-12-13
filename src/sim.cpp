#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <exception>

#include "net.h"
#include "syn.h"

//********************************************************
//             Net Member Functions
//********************************************************

char net::get_signal() {
    if(signal_ == '?') {
        auto it = std::find_if(connections_.begin(), connections_.end(),
            [](pin *p) { return p->get_dir() == 'O'; }
        );
        if(it == connections_.end())
            throw std::runtime_error("floating net");
        pin *driver = *it;
        signal_ = driver->compute_signal();
    }
    return signal_;
}

//********************************************************
//             Pin Member Functions
//********************************************************

char pin::compute_signal() {
    assert(nets_.size() == 1);
    if(dir_ == 'O')
        return gate_->compute_signal(index_);
    else
        return nets_.front()->get_signal();
}

//********************************************************
//             Gate Member Functions
//********************************************************

void gate::compute_state_or_output(std::string file_name) {
    std::cout << "computing state or output of " << type_ << std::endl;
    assert(false);
}

char gate::compute_signal(int pin_index) {
    std::cout << "computing signal for " << type_ << std::endl;
    assert(false);
    return 'X';
}

bool gate::validate_structural_semantics() {
    std::cout << "validating semantics of " << type_ << std::endl;
    assert(false);
    return false;
}

//********************************************************
//             Netlist Member Functions
//********************************************************

void netlist::compute_state_and_outputs(std::string file_name) {
    for(net *n: nets_)
        n->set_signal('?');
    for(gate *g: gates_)
        g->compute_state_or_output(file_name);
}

void netlist::simulate(std::string file_name, int num_cycles) {
    for(int i = 0; i < num_cycles; ++i) {
        compute_state_and_outputs(file_name);
    }
}
