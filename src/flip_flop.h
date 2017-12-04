#ifndef DFFGATE
#define DFFGATE

#include <string>
#include <list>
#include <vector>
#include <map>
#include <assert.h>

#include "net.h"

class flip_flop: public gate{
    char state_, next_state_;
public:
    flip_flop(std::string name)
        : gate("evl_dff", name), state_('0'), next_state_('0') {}

    bool validate_structural_semantics() override;
    void compute_state_or_output() override;
    char compute_signal(int pin_index) override;
}; // class flip_flop

bool flip_flop::validate_structural_semantics() {
    if(pins_.size() != 3)
        return false;
    pins_[0]->set_as_output(); // q
    pins_[1]->set_as_input(); // d
    pins_[2]->set_as_input(); // clk
}

void flip_flop::compute_state_or_output() {
    next_state_ = pins_[1]->compute_signal(); // d
}

char flip_flop::compute_signal(int pin_index) {
    assert(pin_index == 0); // must be q
    return state_;
}

#endif
