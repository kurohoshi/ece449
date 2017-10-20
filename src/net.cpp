#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <cstring>
#include <sstream>

#include "net.h"
#include "syn.h"

//********************************************************
//             Net Member Functions
//********************************************************

void net::append_pin(pin *p) {
    connections_.push_back(p);
}

//********************************************************
//             Pin Member Functions
//********************************************************

bool pin::create(
    gate *g,
    size_t index,
    const evl_pin &p,
    const std::map<std::string, net *> &nets_table) {

    gate_ = g;
    index_ = index;
    if(p.get_msb() == -1) {
        if(p.get_lsb() == -1) {
            if(nets_table.find(p.get_name()) == nets_table.end()) {
                std::cerr << "'" << p.get_name()
                    << "' was not found in nets_table" << std::endl;
                return false;
            }
        } else {
            std::cerr << "something went wrong ..." << std::endl;
            return false;
        }
        //net_->append_pin(p);
    } else if(p.get_msb() >= 0) {
        std::cout << "pin bus not implemented" << std::endl;
        if(p.get_lsb() == -1) {
            // not implemented
        } else if(p.get_lsb() >= 0 && p.get_lsb() <= p.get_msb()) {
            // not implemented
        } else {
            std::cerr << "incorrect lsb pin assignment" << std::endl;
            return false;
        }
    } else {
        std::cerr << "incorrect msb pin assignment" << std::endl;
        return false;
    }

    //if(p.msb == -1) { //a 1-bit wire
    //  net_name = p.name;
    //  net_ = find net_name in nets_table
    //  net_->append_pin(pin);
    //}
    return true;
}

//********************************************************
//             Gate Member Functions
//********************************************************

bool gate::create(
    const evl_component &c,
    const std::map<std::string, net *> &nets_table,
    const evl_wires_table &wires_table) {

    name_ = c.get_name();
    type_ = c.get_type();

    size_t index = 0;
    for(evl_pins::const_iterator pin_ = c.pins_begin(); pin_ != c.pins_end(); ++pin_) {
        if(wires_table.find(pin_->get_name()) != wires_table.end()) {

            pin *p = new pin;
            pins_.push_back(p);
            if(!(p->create(this, index, *pin_, nets_table))) {
                return false;
            }
            index++;
        } else{
            std::cerr << "wire '" << pin_->get_name()
                << "' already exists'" << std::endl;
        }
    }

    //return validate_structural_semantics()
    return true;
}

//********************************************************
//             Netlist Member Functions
//********************************************************

void netlist::create_net(std::string net_name, int width) {
    assert(nets_table_.find(net_name) == nets_table_.end());
    net *n = new net(net_name, width);
    nets_table_[net_name] = n;
    nets_.push_back(n);
}

bool netlist::create(
    const evl_module &module,
    const evl_wires_table &wires_table) {

    name_ = module.get_name();

    for(evl_wires::const_iterator wire = module.wires_begin(); wire != module.wires_end(); ++wire) {
        if(wire->get_width() == 1) {
            create_net(wire->get_name(), wire->get_width());
        } else {
            for(int i = 0; i < wire->get_width(); ++i) {
                std::ostringstream oss;
                oss << wire->get_name() << "[" << i << "]";
                create_net(oss.str(), wire->get_width());
            }
        }
    }

    for(evl_components::const_iterator c = module.components_begin();
        c != module.components_end(); ++c) {

        gate *g = new gate;
        gates_.push_back(g);
        if(!(g->create(*c, nets_table_, wires_table)))
            return false;
    }

    return true;
}

void net::display(
    std::ostream &out) const {


}

void gate::display(
    std::ostream &out) const {


}

void netlist::display(
    std::ostream &out) const {
/*
    out << "module" << name_ << std::endl;
    if(!nets_.empty()) {
        out <<  "nets " << nets_.size() << std::endl;
        for(std::list<net *>::const_iterator net = nets_.begin(); net != nets_.end(); ++net) {
            out << "  net " << *net->get_name() << " " << *net->connections_size() << std::endl;
            for_each(net->connections_begin(), net->connections_end(),
                [&](pin *p) {
                    out << p->gate->name_ << " " << p->gate->type_ << pin position << std::endl;
                }
            );
        }
    }
    if(!gates_.empty()) {
        out << "components " << gates_.size() << std::endl;
        for(std::list<gate *>::const_iterator gate = gates_.begin(); gate != gates_.end(); ++gate) {
            out << "  component " << gate->name_ << " " << gate->pins_.size();
            for_each(gate->pins_.begin(), gate->pins_.end(),
                [&]() {
                    // store each pin, pin width, net names
                }
            )
        }
    }
*/
}

bool netlist::store(
    std::string file_name) const {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display(output_file);
    return true;
}
