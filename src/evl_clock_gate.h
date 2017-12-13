#ifndef EVLCLOCKGATE
#define EVLCLOCKGATE

#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include "net.h"

class evl_clock_gate: public gate{
public:
    evl_clock_gate(std::string name)
        : gate("evl_clock", name) {}

    bool validate_structural_semantics() override;
    char compute_signal(int pin_index) override;
}; // class buffer gate

bool evl_clock_gate::validate_structural_semantics() {
    std::cout << "checking evl_clock semantics" << std::endl;
    if(pins_.size() != 1)
        return false;
    std::cout << "setting evl_clock pin dir" << std::endl;
    pins_[0]->set_as_output();
    return true;
}

char evl_clock_gate::compute_signal(int pin_index) {
    return '1';
}

#endif
