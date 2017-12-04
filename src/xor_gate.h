#ifndef XOR
#define XOR

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "net.h"

class xor_gate: public gate{
public:
    xor_gate(std::string name)
        : gate("xor", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class or gate

bool xor_gate::validate_structural_semantics() {
    if (pins_.size() < 3)
        return false;
    pins_[0]->set_as_output(); // output
    for_each(std::next(pins_.begin()), pins_.end(),
        [](pin *p) { p->set_as_input(); } // inputs
    );
}

char xor_gate::compute_signal(int pin_index) {
    bool logic = false;
    assert(pin_index == 0); // must be q
    for(pin *p: pins_) {
        if(p->compute_signal() == 1)
            logic = not logic;
    }

    if(logic)
        return '1';
    return (logic ? '1' : '0');
}

#endif
