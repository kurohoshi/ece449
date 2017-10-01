#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <list>
#include <algorithm>

#include "structs.h"
#include "store_token.h"

void display_tokens(
    std::ostream &out,
    const evl_tokens &tokens) {

    for(evl_tokens::const_iterator token = tokens.begin();
        token != tokens.end(); ++token) {

        switch(token->type) {
            case evl_token::SINGLE:
                out << "SINGLE " << token->str << std::endl;
                break;
            case evl_token::NAME:
                out << "NAME " << token->str << std::endl;
                break;
            case evl_token::NUMBER:
                out << "NUMBER " << token->str << std::endl;
                break;
            default:
                std::cerr << "Something broke..." << std::endl;
                break;
        }
    }
}

void display_statements(
    std::ostream &out,
    const evl_statements &statements) {

    for(evl_statements::const_iterator statement = statements.begin();
        statement != statements.end(); ++statement) {
        //out << statement->type << " ";
        for_each(statement->tokens.begin(), statement->tokens.end(),
            [&](evl_token t) {
                out << t.str << " ";
        });
        out << std::endl;
    }
}

void display_modules(
    std::ostream &out,
    const evl_modules &modules) {

    for(evl_modules::const_iterator module = modules.begin();
        module != modules.end(); ++module) {

        out << "module " << module->name << std::endl;
        if(!module->wires.empty()) {
            out << "wires " << module->wires.size() << std::endl;
            for_each(module->wires.begin(), module->wires.end(),
                [&](evl_wire w) {
                    out << "  wire " << w.name << " " << w.width << std::endl;
                    
            });
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

bool store_tokens_to_file(
    std::string file_name,
    const evl_tokens &tokens) {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display_tokens(output_file, tokens);
    return true;
}

bool store_modules_to_file(
    std::string file_name,
    const evl_modules &modules) {

    std::ofstream output_file(file_name.c_str());

    if(!output_file) {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }

    display_modules(output_file, modules);
    return true;
}
