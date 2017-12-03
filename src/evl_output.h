#ifndef EVLOUTPUTGATE
#define EVLOUTPUTGATE

#include <string>
#include <list>
#include <vector>

#include "net.h"

class evl_output_gate: public gate{
public:
    evl_output_gate(std::string name)
        : gate("evl_output", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class evl_output gate

bool evl_output_gate::validate_structural_semantics() {
    if(pins_.size())
        std::cout << "Warning: " << type_
            << " has no connected pins" << std::endl;
    for_each(pins_.begin(), pins_.end(),
        [](pin *p) { p->set_as_input() } // inputs
    );
}

void evl_output_gate::compute_next_state_or_output() {
    std::string file__output_name = file_name
        + "." + name_ + ".evl_output";
    std::ofstream out(file_name.c_str());
    if(!std::ifstream(file_name.c_str())) {
        out << pins_.size() << std::endl;
        for(pin *p: pins_)
            out << p->nets_size();
            if(p != pins_.back())
                out << std::endl;
    }
    // collect signal from all pins and write to file
    out << std::endl;
    for(pin *p: pins_) {
        out << p->compute_bus_signal();
        if(p != pins_.back())
            out << " ";
    }
}

char evl_output_gate::compute_signal(int pin_index) {
    return '1';
}

#endif
