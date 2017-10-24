#include <assert.h>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>

#include "lex.h"
#include "syn.h"

bool evl_module::get_module_name(
    evl_tokens &t) {

    if (t.front().get_str() == "module") {
        t.pop_front();
        if(t.front().get_type() == evl_token::NAME) {
            name = t.front().get_str();
            t.pop_front();
        } else {
            std::cerr << "Invalid module declaration" << std::endl;
            return false;
        }
        if (t.front().get_str() == ";") {
            t.pop_front();
            return true;
        } else {
            std::cerr << "Invalid module declaration" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Invalid module declaration" << std::endl;
        return true;
    }
    return false;
}

bool evl_module::add_wire_to_table(
    const evl_wire &w) {

    if(wires_table.find(w.get_name()) != wires_table.end()) {
        std::cerr << "Wire '" << w.get_name()
            << "' is already defined" << std::endl;
        return false;
    }
    wires_table[w.get_name()] = w.get_width();
    return true;
}

bool evl_module::get_wires(
    evl_tokens &t) {

    //std::cout << "getting wires..." << std::endl;

    enum state_type {
        INIT, WIRE, DONE, WIRES, NAME, BUS_BEGIN,
        MSB, COLON, LSB, BUS_DONE
    };

    state_type state = INIT;
    int wire_width = 1;
    for(; !t.empty() && (state != DONE); t.pop_front()) {
        evl_token tok = t.front();
        switch(state) {
            case INIT:
                if(tok.get_str() == "wire") {
                    wire_width = 1;
                    state = WIRE;
                } else {
                    std::cerr << "Need 'wire' but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            case WIRE:
                if(tok.get_type() == evl_token::NAME) {
                    evl_wire *wire = new evl_wire(
                        tok.get_str(),
                        wire_width);
                    wires.push_back(*wire);
                    add_wire_to_table(*wire);

                    state = NAME;
                } else if(tok.get_str() == "[") {
                    state = BUS_BEGIN;
                } else {
                    std::cerr << "Need NAME or '[' but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            case WIRES:
                if(tok.get_type() == evl_token::NAME) {
                    evl_wire *wire = new evl_wire(
                        tok.get_str(),
                        wire_width);
                    wires.push_back(*wire);
                    add_wire_to_table(*wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            case NAME:
                if(tok.get_str() == ",") {
                    state = WIRES;
                } else if(tok.get_str() == ";") {
                    state = DONE;
                } else {
                    std::cerr << "Need ',' or ';' but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            case BUS_BEGIN:
                if(tok.get_type() == evl_token::NUMBER) {
                    wire_width = atoi(tok.get_str().c_str()) + 1;
                    state = MSB;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": Number expected" << std::endl;
                    return false;
                }
                break;
            case MSB:
                if(tok.get_str() == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ':' expected" << std::endl;
                    return false;
                }
                break;
            case COLON:
                if(tok.get_str() == "0") {
                    state = LSB;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": Zero expected" << std::endl;
                    return false;
                }
                break;
            case LSB:
                if(tok.get_str() == "]") {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ']' expected" << std::endl;
                    return false;
                }
                break;
            case BUS_DONE:
                if(tok.get_type() == evl_token::NAME) {
                    evl_wire *wire = new evl_wire(
                        tok.get_str(),
                        wire_width);
                    wires.push_back(*wire);
                    add_wire_to_table(*wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            default: {
                std::cerr << "Invalid State in WIRES" << std::endl;
                return false;
            }
        }
    }

    if(state != DONE) {
        std::cerr << "[WIRE] Something went wrong..." << std::endl;
        return false;
    }

    return true;
}

bool evl_module::get_component(
    evl_tokens &t) {

    enum state_type {
        INIT, TYPE, NAME, PINS, PIN_NAME, BUS,
        MSB, LSB, COLON, BUS_DONE, PINS_DONE, DONE
    };

    //std::cout << "getting component..." << std::endl;

    state_type state = INIT;
    evl_component *component = new evl_component("", "");
    evl_pin *pin = new evl_pin("", -1, -1);
    for(; !t.empty() && (state != DONE); t.pop_front()) {
        evl_token tok = t.front();
        //std::cout << t.str << std::endl;
        switch(state) {
            case INIT: {
                if(tok.get_type() == evl_token::NAME) {
                    component->set_type(tok.get_str());
                    state = TYPE;
                } else {
                    std::cerr << "Need type but found '"
                        << tok.get_str() << "' on line "
                        << tok.get_line() << std::endl;
                    return false;
                }
                break;
            }
            case TYPE: {
                if(tok.get_type() == evl_token::NAME) {
                    component->set_name(tok.get_str());
                    state = NAME;
                } else if(tok.get_str() == "("){
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": NAME or '(' expected" << std::endl;
                    return false;
                }
                break;
            }
            case NAME: {
                if(tok.get_str() == "(") {
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": '(' expected" << std::endl;
                    return false;
                }
                break;
            }
            case PINS: {
                if(tok.get_type() == evl_token::NAME) {
                    pin->set_name(tok.get_str());
                    pin->set_msb(-1);
                    pin->set_lsb(-1);
                    state = PIN_NAME;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": NAME expected" << std::endl;
                    return false;
                }
                break;
            }
            case PIN_NAME: {
                if(tok.get_str() == "[") {
                    state = BUS;
                } else if(tok.get_str() == ")") {
                    component->add_pin(*pin);
                    state = PINS_DONE;
                } else if(tok.get_str() == ",") {
                    component->add_pin(*pin);
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": '[' or ')' or ',' expected" << std::endl;
                    return false;
                }
                break;
            }
            case BUS: {
                if(tok.get_type() == evl_token::NUMBER) {
                    pin->set_msb(atoi(tok.get_str().c_str()));
                    state = MSB;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case MSB: {
                if(tok.get_str() == "]") {
                    state = BUS_DONE;
                } else if(tok.get_str() == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ']' or ':' expected" << std::endl;
                    return false;
                }
                break;
            }
            case COLON: {
                if(tok.get_type() == evl_token::NUMBER) {
                    pin->set_lsb(atoi(tok.get_str().c_str()));
                    state = LSB;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case LSB: {
                if(tok.get_str() == "]") {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ']' expected" << std::endl;
                    return false;
                }
                break;
            }
            case BUS_DONE: {
                if(tok.get_str() == ",") {
                    component->add_pin(*pin);
                    state = PINS;
                } else if(tok.get_str() == ")") {
                    component->add_pin(*pin);
                    state = PINS_DONE;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ',' or ')' expected" << std::endl;
                    return false;
                }
                break;
            }
            case PINS_DONE: {
                if(tok.get_str() == ";") {
                    state = DONE;
                } else {
                    std::cerr << "LINE " << tok.get_line()
                        << ": ';' expected" << std::endl;
                    return false;
                }
                break;
            }
            default: {
                std::cerr << "Invalid state in components" << std::endl;
                return false;
            }
        }
    }

    if(state != DONE) {
        std::cerr << "[COMP] Something went wrong..." << std::endl;
        return false;
    }
    components.push_back(*component);

    return true;
}

bool evl_modules::group(
    evl_tokens &toks) {

    for(; !toks.empty();) {
        if(toks.front().get_type() != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '"
            << toks.front().get_str() << "' on line"
            << toks.front().get_line() << std::endl;
        }

        evl_module module;
        if(toks.front().get_str() == "module") {
            if(!module.get_module_name(toks)) {
                return false;
            }
        } else {
            std::cerr << "MODULE Expected" << std::endl;
            return false;
        }

        for(; (!toks.empty()) && (toks.front().get_str() != "endmodule");) {
            if(toks.front().get_type() != evl_token::NAME) {
                std::cerr << "Need a NAME token but found '"
                    << toks.front().get_str() << "' on line"
                    << toks.front().get_line() << std::endl;
                return false;
            }

            if(toks.front().get_str() == "wire") {
                if(!module.get_wires(toks))
                    return false;
                continue;
            } else {
                if(!module.get_component(toks))
                    return false;
                continue;
            }
        }

        if((!toks.empty()) && (toks.front().get_str() == "endmodule")) {
            toks.pop_front();
            modules.push_back(module);
        } else {
            std::cerr << "ENDMODULE expected" << std::endl;
            return false;
        }
    }
    return true;
}

void evl_component::display(
    std::ostream &out) const {

    out << "  component " << type << " ";
    if(name != "") {
        out << name << " ";
    }
    out << pins.size()  << std::endl;
    for(evl_pins::const_iterator pin = pins.begin();
        pin != pins.end(); ++pin) {
            out << "    pin " << pin->get_name();
        if(pin->get_msb() != -1) {
            out << " " << pin->get_msb();
        }
        if(pin->get_lsb() != -1) {
            out << " " << pin->get_lsb();
        }
            out << std::endl;
        }
}

void evl_module::display(
    std::ostream &out) const {

    out << "module " << name << std::endl;
    if(!wires.empty()) {
        out << "wires " << wires.size() << std::endl;
        for_each(wires.begin(), wires.end(),
            [&](evl_wire w) {
                out << "  wire " << w.get_name() << " "
                    << w.get_width() << std::endl;
            }
        );
    }
    if(!components.empty()) {
        out << "components " << components.size() << std::endl;
        for(evl_components::const_iterator component = components.begin();
            component != components.end(); ++component) {

            component->display(out);
        }
    }
}

void evl_modules::display(
    std::ostream &out) const {

    for(evl_modules_::const_iterator module = modules.begin();
        module != modules.end(); ++module) {

        module->display(out);
    }
}

bool evl_modules::store(
    std::string file_name) const {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display(output_file);
    return true;
}
