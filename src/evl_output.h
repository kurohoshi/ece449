#ifndef EVLOUTPUTGATE
#define EVLOUTPUTGATE

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "net.h"

class evl_output_gate: public gate{
public:
    evl_output_gate(std::string name)
        : gate("evl_output", name) {}

    bool validate_structural_semantics();
    void compute_state_or_output(std::string file_name);
}; // class evl_output gate

bool evl_output_gate::validate_structural_semantics() {
    if(pins_.size())
        std::cout << "Warning: " << type_
            << " has no connected pins" << std::endl;
    for_each(pins_.begin(), pins_.end(),
        [](pin *p) { p->set_as_input(); } // inputs
    );
    return true;
}

void evl_output_gate::compute_state_or_output(std::string file_name) {
    std::string file_output_name = file_name
        + "." + name_ + ".evl_output";
    std::ofstream out(file_name.c_str());
    if(!std::ifstream(file_name.c_str())) {
        out << pins_.size() << std::endl;
        for(pin *p: pins_) {
            out << p->nets_size();
            if(p != pins_.back()) {
                out << std::endl;
            }
        }
    }
    // collect signal from all pins and write to file
    out << std::endl;
    for(pin *p: pins_) {
        int hex_num = 0;
        int hex_width = (p->nets_size() / 4)
            + ((p->nets_size() % 4) != 0 ? 1 : 0);

        for(net *n: p->get_nets()) {
            hex_num *= 2;
            hex_num += (n->get_signal() == '1') ? 1 : 0;
        }

        std::stringstream ss;
        ss << std::hex << std::setw(hex_width) << std::setfill('0') << hex_num;
        out << ss.str();

        if(p != pins_.back())
            out << " ";
    }
}

#endif
