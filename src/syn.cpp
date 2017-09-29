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
            std::cout << "grouping module statement" << std::endl;
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
            std::cout << "grouping endmodule statement" << std::endl;

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
            std::cout << "grouping wire statement" << std::endl;
                
            evl_statement wire;
            wire.type = evl_statement::WIRE;

            if(!get_statement(wire.tokens, tokens))
                return false;

            statements.push_back(wire);
        } else {
            if(statements.empty()) {
                std::cerr << "LINE " << token.line_no
                    << ": module not declared" << std::endl;
            }
            std::cout << "grouping component statement" << std::endl;

            evl_statement component;
            component.type = evl_statement::COMPONENT;

            if(!get_statement(component.tokens, tokens))
                return false;

            statements.push_back(component);
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

bool get_module_name(
    std::string &name,
    evl_statement &s);

bool get_wires(
    evl_wires &wires,
    evl_statement &s);

bool get_component(
    evl_components &components,
    evl_statement &s);

bool analyze_statements(
    evl_statements &statements,
    evl_modules &modules) {

    assert(modules.empty());

    for(; !statements.empty(); statements.pop_front()) {
        evl_statement statement = statements.front();

        evl_module module;

        if(statement.type == evl_statement::MODULE) {
            if(!get_module_name(module.name, statement))
                return false;
            statements.pop_front();
            if(statements.empty()) {
                std::cerr << "Expected something but got nothing" << std::endl;
            }
            statement = statements.front();
        } else {
            std::cerr << "MODULE Expected" << std::endl;
            return false;
        }

        for(; (!statements.empty()) && (statement.type != evl_statement::ENDMODULE);
            /*statements.pop_front()*/) {

            //statement = statements.front();

            if(statement.type == evl_statement::ENDMODULE)
                std::cout << evl_statement::MODULE  << std::endl;

            switch(statement.type) {
                case evl_statement::WIRE: {
                    if(!get_wires(module.wires, statement))
                        return false;
                    break;
                }
                case evl_statement::COMPONENT: {
                    if(!get_component(module.components, statement))
                        return false;
                    break;
                }
                default: {
                    std::cout << "[EMOD] Something went wrong..." << std::endl;
                    return false;
                }
            }
            statements.pop_front();
            statement = statements.front();
        }

        if((!statements.empty()) && (statement.type == evl_statement::ENDMODULE)) {
            modules.push_back(module);
        } else {
            std::cerr << "ENDMODULE expected" << std::endl;
            return false;
        }
    }

    return true;
}

bool get_module_name(
    std::string &name,
    evl_statement &s) {

    std::cout << "getting module name..." << std::endl;

    if (s.tokens.front().str == "module" && s.tokens.back().str == ";") {
        s.tokens.pop_front(); s.tokens.pop_back();
        name = s.tokens.front().str;
        s.tokens.pop_front();
    }
    if (!s.tokens.empty()) {
        std::cerr << "Invalid module declaration" << std::endl;
    }

    return true;
}

bool get_wires(
    evl_wires &wires,
    evl_statement &s) {

    std::cout << "getting wires..." << std::endl;

    enum state_type 
        {INIT, WIRE, DONE, WIRES, NAME, BUS_BEGIN, MSB, COLON, LSB, BUS_DONE};

    state_type state = INIT;
    int wire_width = 1;
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
                std::cout << evl_token::NUMBER << std::endl;
                std::cout << t.type << std::endl;
                if(t.type == evl_token::NUMBER) {
                    wire_width = atoi(t.str.c_str()) + 1;
                    state = MSB;
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
            default: {
                std::cerr << "Invalid State in WIRES" << std::endl;
                return false;
            }
        }
    }

    if(!s.tokens.empty() || (state != DONE)) {
        std::cerr << "[WIRE] Something went wrong..." << std::endl;
        return false;
    }

    return true;
}

bool get_component(
    evl_components &components,
    evl_statement &s) {

    enum state_type
        {INIT, TYPE, NAME, PINS, PIN_NAME, BUS, MSB, LSB, COLON, BUS_DONE, PINS_DONE, DONE};

    std::cout << "getting component..." << std::endl;

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
                break;
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
                break;
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
                break;
            }
            case BUS: {
                if(t.type == evl_token::NUMBER) {
                    pin.bus_msb = atoi(t.str.c_str());
                    state = MSB;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case MSB: {
                if(t.str == "]") {
                    state = BUS_DONE;
                } else if(t.str == ":") {
                    state = COLON;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ']' or ':' expected" << std::endl;
                    return false;
                }
                break;
            }
            case COLON: {
                if(t.type == evl_token::NUMBER) {
                    pin.bus_lsb = atoi(t.str.c_str());
                    state = BUS_DONE;
                } else {
                    std::cerr << "LINE " << t.line_no << ": NUMBER expected" << std::endl;
                    return false;
                }
                break;
            }
            case BUS_DONE: {
                if(t.str == ",") {
                    component.pins.push_back(pin);
                    state = PINS;
                } else if(t.str == ")") {
                    component.pins.push_back(pin);
                    state = PINS_DONE;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ',' or ')' expected" << std::endl;
                    return false;
                }
                break;
            }
            case PINS_DONE: {
                if(t.str == ";") {
                    state = DONE;
                } else {
                    std::cerr << "LINE " << t.line_no << ": ';' expected" << std::endl;
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

    if(!s.tokens.empty() || (state != DONE)) {
        std::cerr << "[COMP] Something went wrong..." << std::endl;
        return false;
    }

    return true;
}
