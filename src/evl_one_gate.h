#ifndef EVLONEGATE
#define EVLONEGATE

#include <string>
#include <list>
#include <vector>

#include "net.h"

class evl_one_gate: public gate{
public:
    evl_one_gate(std::string name)
        : gate("evl_one", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class buffer gate

bool evl_one_gate::validate_structural_semantics() {
    if(pins_.size())
        return false;
    for_each(pins_.begin(), pins_.end(),
        [](pin *p) { p->set_as_output() } // inputs
    );
}

char evl_one_gate::compute_signal(int pin_index) {
    return '1';
}

#endif
