#include <assert.h>
#include <string>
#include <list>
#include <iostream>
#include <algorithm>

#include "structs.h"
#include "syn.h"

bool get_statement(
    evl_tokens &statement_tokens,
    evl_tokens &tokens);

bool group_tokens_into_statements(
    evl_statements &statements,
    evl_tokens &tokens) {

    for(; !tokens.empty();) {
        evl_token token = tokens.front();
        if(token.type != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << token.str
                << "' on line" << token.line_no << std::endl;
        }

        if(token.str == "module") {
            evl_statement module;
            module.type = evl_statement::MODULE;

            if(!get_statement(module.tokens, tokens))
                return false;

            statements.push_back(module);
        } else if(token.str == "endmodule") {
            if(statements.empty()) {
                std::cerr << "LINE " << token.line_no
                    << ": module no declared" << std::endl;
            }

            evl_statement endmodule;

            endmodule.type = evl_statement::ENDMODULE;
            endmodule.tokens.push_back(token);

            tokens.pop_front();
            statements.push_back(endmodule);
        } else if(token.str == "wire") {
            if(statements.empty()) {
                std::cerr << "LINE " << token.line_no
                    << ": module not declared" << std::endl;
            }
                
            evl_statement wire;

            if(!get_statement(wire.tokens, tokens))
                return false;

            statements.push_back(wire);
        } else {
            continue;
        }
    }

    return true;
} 

bool get_statement(
    evl_tokens &statement_tokens,
    evl_tokens &tokens) {

    assert(statement_tokens.empty());

    evl_tokens::iterator next_sc = std::find_if(
        tokens.begin(), tokens.end(),
        [](evl_token token) { return token.str == ";"; });
    if(next_sc == tokens.end()) {
        std::cerr << "Look for ';' but reach the end of file" << std::endl;
        return false;
    }

    evl_tokens::iterator after_sc = next_sc; ++after_sc;
    statement_tokens.splice(statement_tokens.begin(),
        tokens, tokens.begin(), after_sc);

    return true;
}

bool analyze_statement(
    evl_statements &statements
    evl_modules &modules) {

    assert(modules.empty());
    bool flag = false;
    for(; statements.empty(); statements.pop_front()) {
        evl_statement statement = statements.front();
        switch(statement->type) {
            case MODULE: {
                evl_module module;
                get_module_name(module.name, statement);
                flag = true;
            }
            case WIRE: {
                assert(flag);
                get_wires(module.wires, statement);
                break;
            }
            case COMPONENT: {
                assert(flag);
                get_component(module.components, statement);
            }
            case ENDMODULE: {
                modules.push_back(module);
                flag = false;
                break;
            }
            default: {
                std::cout << "Something went wrong..." << std::endl;
            }
    //iterate thru statements
    //if module, make module?
    // if not module, then throw error
    //if wire, check for module, then get wires
    //if get components
    //if end the module
        }
    }

    if(flag) {
        std::cout << "ENDMODULE not found" << std::endl;
        return false;
    }
}

bool get_wires(
    evl_wires &wires,
    evl_statement &s) {

    enum state_type 
        {INIT, WIRE, DONE, WIRES, NAME, BUS_BEGIN, MSB, COLON, LSB, BUS_DONE};

    state_type state = INIT;
    int wire_width;
    for(; !s.tokens.empty() && (state != DONE); s.tokens.pop_front()) {
        evl_token t = s.tokens.front();
        switch(state) {
            case INIT:
                if(t.str == "wire") {
                    wire_width = 1;
                    state = WIRE;
                } else {
                    std::cerr << "Need 'wire' but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            case WIRE:
                if(t.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = t.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else if(t.str == "[") {
                    state = BUS_BEGIN;
                } else {
                    std::cerr << "Need NAME or '[' but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            case WIRES:
                if(t.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = t.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            case NAME:
                if(t.str == ",") {
                    state = WIRES;
                } else if(t.str == ";") {
                    state = DONE;
                } else {
                    std::cerr << "Need ',' or ';' but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            case BUS_BEGIN:
                if(t.type == evl_token::NUMBER) {
                    wire_width = atoi(t.str.c_str()) + 1;
                } else {
                    std::cerr << "LINE " << t.line_no << ": Number expected" << std::endl;
                    return false;
                }
                break;
            case MSB:
                if(t.str == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ':' expected" << std::endl;
                    return false;
                }
                break;
            case COLON:
                if(t.str == "0") {
                    state = LSB;
                } else {
                    std::cerr << "LINE " << t.line_no << ": Zero expected" << std::endl;
                    return false;
                }
                break;
            case LSB:
                if(t.str == "]") {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ']' expected" << std::endl;
                    return false;
                }
                break;
            case BUS_DONE:
                if(t.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = t.str; wire.width = wire_width;
                    wires.push_back(wire);

                    state = NAME;
                } else {
                    std::cerr << "Need NAME but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            default:
                std::cerr << "Invalid State in WIRES" << std::endl;
                return false;
        }
    }

    if(!s.tokens.empty() || (state != DONE)) {
        std::cerr << "something wrong with the statement" << std::endl;
        return false;
    }

    return true;
}

get_component(
    evl_components &components,
    evl_statement &s) {

    enum state_type
        {INIT, TYPE, NAME, PINS, PIN_NAME, BUS, MSB, LSB, COLON, BUS_DONE, PINS_DONE, DONE};

    state_type state = INIT;
    evl_component component;
    evl_pin pin;
    for(; !s.tokens.empty() && (state != DONE); s.tokens.pop_front()) {
        evl_token t = s.tokens.front();
        switch(state) {
            case INIT: {
                if(t.type == evl_token::NAME) {
                    component.type = t.str;
                    component.name = "";
                    state = TYPE;
                } else {
                    std::cerr << "Need name but found '" << t.str
                        << "' on line " << t.line_no << std::endl;
                    return false;
                }
                break;
            }
            case TYPE: {
                if(t.type == evl_token::NAME) {
                    component.name = t.str;
                    state = NAME;
                } else if(t.str == "("){
                    state = PINS;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NAME or '(' expected" << std::endl;
                    return false;
                }
                break;
            }
            case NAME: {
                if(t.str == "(") {
                    state = PINS;
                } else {
                    std::cerr << "LINE " << t.line_no << ": '(' expected" << std::endl;
                    return false;
                }
            }
            case PINS: {
                if(t.type == evl_token::NAME) {
                    pin.name = t.str;
                    pin.bus_msb = -1;
                    pin.bus_lsb = -1;
                    state = PIN_NAME;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NAME expected" << std::endl;
                    return false;
                }
            }
            case PIN_NAME: {
                if(t.str == "[") {
                    state = BUS;
                } else if(t.str == ")") {
                    component.pins.push_back(pin);
                    state = PINS_DONE;
                } else if(t.str == ",") {
                    component.pins.push_back(pin);
                    state = PINS;
                } else {
                    std::cerr << "LINE " << t.line_no << ": '[' or ')' or ',' expected" << std::endl;
                    return false;
                }
            }
            case BUS: {
                if(t.type == evl_token::NUMBER) {
                    pin.bus_msb = atoi(t.str.c_str());
                    state = BUS_MSB;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NUMBER expected" << std::endl;
                    return false;
                }
            }
            case MSB: {
                if(t.str == "]") {
                    state = BUS_DONE;
                } else if(t.str == ":") {
                    state = BUS_COLON;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ']' or ':' expected" << std::endl;
                    return false;
                }
            }
            case COLON: {
                if(t.type == evl_token::NUMBER) {
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NUMBER expected" << std::endl;
            }
            case BUS_DONE: {

            }
            case PINS_DONE: {

            }
            case DONE: {

            }
        }
    }
}
