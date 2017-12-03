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
    if(dir_ == 'O')
        return gate_->compute_signal(index_);
    else
        return net_->get_signal();
}

//********************************************************
//             Gate Member Functions
//********************************************************

void gate::compute_state_or_output() {
    if(type_ == "evl_dff") {
        next_state_ = pins_[1]->compute_signal() // d
    } else if(type == "evl_output") {
        std::string file_name = // file name here;
        std::ofstream out(file_name.c_str());
        if(!std::ifstream(file_name.c_str())) {
            out << pins_.size() << std::endl;
            for(pin *p: pins_)
                out << p->nets_size() << std::endl;
        }
        // collect signal from all pins and write to file
        for(pin *p, pins_) {
            out << p->compute_signal();
            if(p != pins_.back())
                out << " ";
        }
        out << std::endl;
    }
}

bool gate::compute_signal(int pin_index) {
    if(type_ == "evl_dff") {
        assert pin_index == 0; // must be q
        return state_;
    } else if(type_ == "evl_zero") {
        return 0;
    } else if(type_ == "evl_one") {
        return 1;
    } else if(type_ == "and") {
        assert pin_index = 0 // must be out
        // collect signals from the input pins
        // compute and return the output signal
    } else if(type_ == "or") {
        assert pin_index = 0 // must be out
        // collect signals from the input pins
        // compute and return the output signal
    } else if(type_ == "xor") {
        assert pin_index = 0 // must be out
        // collect signals from the input pins
        // compute and return the output signal
    } else if(type_ == "not") {
        assert pin_index = 0 // must be out
        // collect signals from the input pins
        // compute and return the output signal
    } else if(type_ == "buf") {
        assert pin_index = 0 // must be out
        // collect signals from the input pins
        // compute and return the output signal
    } // etc
    return -1;
}

bool gate::validate_structural_semantics() {
    if(type_ == "and") {
        if(pins_.size() < 3)
            return false;
        pins_[0]->set_as_output(); // output
        for(size_t i = 1; i < pins_.size(); ++i)
            pins[i]->set_as_input();
    } else if(type_ == evl_dff) {
        if(pins_.size() != 3)
            return false;
        pins_[0]->set_as_output(); //q
        pins_[1]->set_as_input(); //q
        pins_[2]->set_as_input(); //q
    } //etc
}

//********************************************************
//             Netlist Member Functions
//********************************************************

void netlist::compute_state_and_output() {
    for(net *n: nets_)
        n->set_signal('?');
    for(gate *g: gates_)
        g->compute_state_or_output();
}
