#ifndef EVL_TOKEN
#define EVL_TOKEN

struct evl_token{
    enum token_type {NAME, NUMBER, SINGLE};

    token_type type;
    std::string str;

    int line_no;
}; //struct evl_token

#endif
