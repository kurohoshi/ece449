#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>

#include "net.h"
#include "syn.h"

//********************************************************
//             Net Member Functions
//********************************************************

char net::get_signal() {
    if(signal_ == '?') {
        auto it = std::find_if(connections_.begin(), connections_.end(),
            [](pin *p) { p->get_dir() == 'O'; }
        );
        if(it == connections_.end())
            throw std::runtime_exception("floating net");
        pin *driver = *it;
        signal_ = driver->compute_signal();
    }
    return signal_;
}

void net::set_signal(char c) {
    signal_ = c;
}

//********************************************************
//             Pin Member Functions
//********************************************************

char pin::compute_signal() {
    assert(nets_.size() == 1)
    if(dir_ == 'O')
        return gate_->compute_signal(index_);
    else
        return net_->get_signal();
}

std::string pin::compute__bus_signal() {

}

//********************************************************
//             Gate Member Functions
//********************************************************

void gate::compute_state_or_output(std::string file_name) {
    assert(false);
}

bool gate::compute_signal(int pin_index) {
    assert(false);
    return 'X';
}

bool gate::validate_structural_semantics() {
    assert(false);
    return false;
}

//********************************************************
//             Netlist Member Functions
//********************************************************

void netlist::compute_state_and_output(std::string file_name) {
    for(net *n: nets_)
        n->set_signal('?');
    for(gate *g: gates_)
        g->compute_state_or_output(file_name);
}
