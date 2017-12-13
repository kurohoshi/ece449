#ifndef BUFGATE
#define BUFGATE

#include <string>
#include <list>
#include <vector>
#include <assert.h>

#include "net.h"

class buf_gate: public gate{
public:
    buf_gate(std::string name)
        : gate("buf", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class buffer gate

bool buf_gate::validate_structural_semantics() {
    if (pins_.size() != 2)
        return false;
    pins_[0]->set_as_output(); // o
    pins_[1]->set_as_input(); // i
    return true;
}

char buf_gate::compute_signal(int pin_index) {
    assert(pin_index == 0); // must be q
    return pins_[1]->compute_signal();
}

#endif
