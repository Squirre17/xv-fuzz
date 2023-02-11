#include "str.h"

/*
    @brief: split string by substring
*/
vector<string_view> split(const string &content, string_view delim_str) {
    size_t tail_pos = 0;
    size_t head_pos = std::string::npos;
    vector<string_view> str_view_vec;

    while((head_pos = content.find(delim_str, tail_pos)) 
            != std::string::npos) 
    {
        str_view_vec.emplace_back(&content[tail_pos], head_pos - tail_pos);
        tail_pos = head_pos + delim_str.length();
    }
    if(tail_pos != content.length() - 1) {
        str_view_vec.emplace_back(&content[tail_pos], content.length() - tail_pos);
    }
    return str_view_vec;
}
void ut1() {

    string s("123 12312 123213a as dasd as asd");
    std::cout << string_view("asdasd 123") << std::endl;
    vector<string_view> l = split(s, string_view(" "));

    for(const auto &i : l) {
        std::cout << i << std::endl;
    }
    
}