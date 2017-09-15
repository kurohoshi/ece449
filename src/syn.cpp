#include <string>
#include <vector>

void syntax() {
    //check module, then wires/components, then endmodule
    //check module and endmodule first, then wires/components
    //if(/*check if module and endmodule exists*/){
    //    std::cerr << "Syntax Error: Need module or endmodule" << std::endl;
    //}
    /*
    if name encountered, check if NAME = module
    then check if next token = NAME and NAME = top
    then check for ;

    check for NAME
        if NAME = endmodule, end of module
        if NMAE = wire, create wire type
            if next token != NAME then syntax error
            if next token = NAME record NAME under wire and record bus width
                check for , or ; SINGLE
                if , then check if next is NAME
                if ; then end list of wires
        if NAME = neither module, endmodule, or wire, make component
        check opening parenthesis
            if none, then throw syntax error
            check for name or closing parenthesis next
                if name, record name
                if closing parenthesis, check for ;
                    if no ;, throw syntax error
         if not NAME, then throw error
    */
}

void comp_syntax() {

}

void wire_syntax() {

}
