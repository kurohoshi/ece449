#ifndef ANDGATE
#define ANDGATE

#include <string>
#include <list>
#include <vector>

#include "net.h"

class and_gate: public gate{
public:
    and_gate(std::string name)
        : gate("and", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class or gate

bool and_gate::validate_structural_semantics() {
    if (pins_.size() < 3)
        return false;
    pins_[0]->set_as_output(); // output
    for_each(pins_.begin().next(), pins_.end(),
        [](pin *p) { p->set_as_input() } // inputs
    );
}

char and_gate::compute_signal(int pin_index) {
    assert pin_index == 0; // must be q
    if(all_of(pins_.begin().next(), pins_.end(),
        [](pin *p) { return p->compute_signal() == '1'; }))
        return '1';
    return '0';
}

#endif
