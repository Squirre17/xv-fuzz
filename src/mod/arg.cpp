
#include "arg.h"

Option::Option(const std::string &key, const std::string &des) 
: key(key), des(des) {}

/*
    @brief : maintain option table manually
*/
const std::vector<Option> KW::options_table(){
    std::vector<Option> tab;

    tab.push_back(Option(KW::INPUT          , "The path of input corpus(directory)"));
    tab.push_back(Option(KW::OUTPUT         , "The path of output sample(directory)"));
    tab.push_back(Option(KW::TIMEOUT_SPAN   , "The timeout span of fuzzee"));

    return tab;
}
/*
    @brief  : parse cmd line args 
*/

Args &Args::parse_args(int argc, char *argv[]) {

    auto opt_table = KW::options_table();

    if((argc % 2 != 0) or (argc < 2)) {
        /* not according one-to-one key-value pair */
        // TODO: more hint
        throw "arguments error";
    }

    this->fuzzee = std::string(argv[1]);
    // TODO: const it
    std::map<std::string, std::string> user_ops;
    for (int i = 2; i < argc - 1; i += 2) {
        std::string key = std::string(argv[i]);
        std::string val = std::string(argv[i + 1]);
        user_ops[key] = val;
    }

    using map_s2s_it = std::map<std::string, std::string>::const_iterator;
    for(map_s2s_it it = user_ops.begin(); it != user_ops.end(); it++) {

        bool is_vaild = false;
        /* not check argument's validation here */
        for(const auto &op : opt_table) {
            if((*it).first == op.key){
                is_vaild = true;
                break;
                // TODO:
            }
        }
        if (not is_vaild) {
            // TODO: more hint
            throw "nota vaild argument";
        }

    }
    this->user_ops = user_ops;
    return *this;
}

Args &Args::process() {
    // TODO: need path lib to open a file 
    return *this;
}
