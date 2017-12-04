#ifndef EVLZEROGATE
#define EVLZEROGATE

#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include "net.h"

class evl_zero_gate: public gate{
public:
    evl_zero_gate(std::string name)
        : gate("evl_zero", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class buffer gate

bool evl_zero_gate::validate_structural_semantics() {
    if(pins_.size())
        return false;
    for_each(pins_.begin(), pins_.end(),
        [](pin *p) { p->set_as_output(); } // inputs
    );
}

char evl_zero_gate::compute_signal(int pin_index) {
    return '0';
}

#endif
