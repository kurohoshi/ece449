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

bool evl_modules::get_module_name(
    std::string &name,
    evl_tokens &t) {

    if (t.tokens.front().str == "module") {
        t.tokens.pop_front();
        if(t.tokens.front().type == evl_token::NAME) {
            name = t.tokens.front().str;
            t.tokens.pop_front();
        } else {
            std::cerr << "Invalid module declaration" << std::endl;
            return false;
        }
        if (t.tokens.front().str == ";") {
            t.tokens.pop_front();
            return true;
        } else {
            std::cerr << "Invalid module declaration" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Invalid module declaration" << std::endl;
        return false;
    }
    return false;
}

bool evl_modules::get_wires(
    evl_wires &wires,
    evl_tokens &t) {

    //std::cout << "getting wires..." << std::endl;

    enum state_type
        {INIT, WIRE, DONE, WIRES, NAME, BUS_BEGIN, MSB, COLON, LSB, BUS_DONE};

    state_type state = INIT;
    int wire_width = 1;
    for(; !t.tokens.empty() && (state != DONE); t.tokens.pop_front()) {
        evl_token tok = t.tokens.front();
        switch(state) {
            case INIT:
                if(tok.str == "wire") {
                    wire_width = 1;
                    state = WIRE;
                } else {
                    std::cerr << "Need 'wire' but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
                    return false;
                }
                break;
            case WIRE:
                if(tok.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = tok.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else if(tok.str == "[") {
                    state = BUS_BEGIN;
                } else {
                    std::cerr << "Need NAME or '[' but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
                    return false;
                }
                break;
            case WIRES:
                if(tok.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = tok.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
                    return false;
                }
                break;
            case NAME:
                if(tok.str == ",") {
                    state = WIRES;
                } else if(tok.str == ";") {
                    state = DONE;
                } else {
                    std::cerr << "Need ',' or ';' but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
                    return false;
                }
                break;
            case BUS_BEGIN:
                if(tok.type == evl_token::NUMBER) {
                    wire_width = atoi(tok.str.c_str()) + 1;
                    state = MSB;
                } else {
                    std::cerr << "LINE " << tok.line_no << ": Number expected" << std::endl;
                    return false;
                }
                break;
            case MSB:
                if(tok.str == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << tok.line_no << ": ':' expected" << std::endl;
                    return false;
                }
                break;
            case COLON:
                if(tok.str == "0") {
                    state = LSB;
                } else {
                    std::cerr << "LINE " << tok.line_no << ": Zero expected" << std::endl;
                    return false;
                }
                break;
            case LSB:
                if(tok.str == "]") {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << tok.line_no << ": ']' expected" << std::endl;
                    return false;
                }
                break;
            case BUS_DONE:
                if(tok.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = tok.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
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

bool evl_modules::get_component(
    evl_components &components,
    evl_tokens &t) {

    enum state_type {
        INIT, TYPE, NAME, PINS, PIN_NAME, BUS,
        MSB, LSB, COLON, BUS_DONE, PINS_DONE, DONE
    };

    //std::cout << "getting component..." << std::endl;

    state_type state = INIT;
    evl_component component;
    evl_pin pin;
    for(; !t.tokens.empty() && (state != DONE); t.tokens.pop_front()) {
        evl_token tok = t.tokens.front();
        //std::cout << t.str << std::endl;
        switch(state) {
            case INIT: {
                if(tok.type == evl_token::NAME) {
                    component.type = tok.str;
                    component.name = "";
                    state = TYPE;
                } else {
                    std::cerr << "Need name but found '" << tok.str
                        << "' on line " << tok.line_no << std::endl;
                    return false;
                }
                break;
            }
            case TYPE: {
                if(tok.type == evl_token::NAME) {
                    component.name = tok.str;
                    state = NAME;
                } else if(tok.str == "("){
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": NAME or '(' expected" << std::endl;
                    return false;
                }
                break;
            }
            case NAME: {
                if(tok.str == "(") {
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": '(' expected" << std::endl;
                    return false;
                }
                break;
            }
            case PINS: {
                if(tok.type == evl_token::NAME) {
                    pin.name = tok.str;
                    pin.bus_msb = -1;
                    pin.bus_lsb = -1;
                    state = PIN_NAME;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": NAME expected" << std::endl;
                    return false;
                }
                break;
            }
            case PIN_NAME: {
                if(tok.str == "[") {
                    state = BUS;
                } else if(tok.str == ")") {
                    component.pins.push_back(pin);
                    state = PINS_DONE;
                } else if(tok.str == ",") {
                    component.pins.push_back(pin);
                    state = PINS;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": '[' or ')' or ',' expected" << std::endl;
                    return false;
                }
                break;
            }
            case BUS: {
                if(tok.type == evl_token::NUMBER) {
                    pin.bus_msb = atoi(tok.str.c_str());
                    state = MSB;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case MSB: {
                if(tok.str == "]") {
                    state = BUS_DONE;
                } else if(tok.str == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": ']' or ':' expected" << std::endl;
                    return false;
                }
                break;
            }
            case COLON: {
                if(tok.type == evl_token::NUMBER) {
                    pin.bus_lsb = atoi(tok.str.c_str());
                    state = LSB;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case LSB: {
                if(tok.str == "]") {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": ']' expected" << std::endl;
                    return false;
                }
                break;
            }
            case BUS_DONE: {
                if(tok.str == ",") {
                    component.pins.push_back(pin);
                    state = PINS;
                } else if(tok.str == ")") {
                    component.pins.push_back(pin);
                    state = PINS_DONE;
                } else {
                    std::cerr << "LINE " << tok.line_no
                        << ": ',' or ')' expected" << std::endl;
                    return false;
                }
                break;
            }
            case PINS_DONE: {
                if(tok.str == ";") {
                    state = DONE;
                } else {
                    std::cerr << "LINE " << tok.line_no
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
    components.push_back(component);

    return true;
}

bool evl_modules::group(
    evl_tokens &toks) {

    for(; !toks.tokens.empty();) {
        if(toks.tokens.front().type != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << toks.tokens.front().str
                << "' on line" << toks.tokens.front().line_no << std::endl;
        }

        evl_module module;
        if(toks.tokens.front().str == "module") {
            if(!get_module_name(module.name, toks)) {
                return false;
            }
        } else {
            std::cerr << "MODULE Expected" << std::endl;
            return false;
        }

        for(; (!toks.tokens.empty()) && (toks.tokens.front().str != "endmodule");) {
            if(toks.tokens.front().type != evl_token::NAME) {
                std::cerr << "Need a NAME token but found '" << toks.tokens.front().str
                    << "' on line" << toks.tokens.front().line_no << std::endl;
                return false;
            }

            if(toks.tokens.front().str == "wire") {
                if(!get_wires(module.wires, toks))
                    return false;
                continue;
            } else {
                if(!get_component(module.components, toks))
                    return false;
                continue;
            }
        }

        if((!toks.tokens.empty()) && (toks.tokens.front().str == "endmodule")) {
            modules.push_back(module);
        } else {
            std::cerr << "ENDMODULE expected" << std::endl;
            return false;
        }
    }
    return true;
}

void evl_modules::display(
    std::ostream &out) const {

    for(evl_modules_::const_iterator module = modules.begin();
        module != modules.end(); ++module) {

        out << "module " << module->name << std::endl;
        if(!module->wires.empty()) {
            out << "wires " << module->wires.size() << std::endl;
            for_each(module->wires.begin(), module->wires.end(),
                [&](evl_wire w) {
                    out << "  wire " << w.name << " " << w.width << std::endl;
                }
            );
        }
        if(!module->components.empty()) {
            out << "components " << module->components.size() << std::endl;
            for(evl_components::const_iterator component = module->components.begin();
                component != (module->components.end()); ++component) {

                out << "  component " << component->type << " ";
                if(component->name != "") {
                    out << component->name << " ";
                }
                out << component->pins.size() << std::endl;

                for(evl_pins::const_iterator pin = component->pins.begin();
                    pin != (component->pins.end()); ++pin) {

                    out << "    pin " << pin->name;
                    if(pin->bus_msb != -1) {
                        out << " " << pin->bus_msb;
                    }
                    if(pin->bus_lsb != -1) {
                        out << " " << pin->bus_lsb;
                    }
                    out << std::endl;
                }
            }
        }
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

bool make_wires_table(
    const evl_wires &wires,
    evl_wires_table &wires_table) {

    for(auto &wire: wires) {
        auto same_name = wires_table.find(wire.name);
        if(same_name != wires_table.end()) {
            std::cerr << "Wire '" << wire.name
                << "' is already defined" << std::endl;
            return false;
        }
        wires_table[wire.name] = wire.width;
    }
    return true;
}
