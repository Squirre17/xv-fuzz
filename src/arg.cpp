
#include "arg.h"
/*
    @brief : maintain option table manually
*/
Option::Option(const std::string &key, const std::string &des) 
: key(key), val(val) {}

const std::vector<const Option> KW::options_table(){
    std::vector<const Option> tab;

    tab.push_back(Option(KW::INPUT          , "The path of input corpus(directory)"));
    tab.push_back(Option(KW::OUTPUT         , "The path of output sample(directory)"));
    tab.push_back(Option(KW::TIMEOUT_SPAN   , "The timeout span of fuzzee"));

    return tab;
}
/*
    @brief  : parse cmd line args 
    @return : option k-v map
*/
std::optional<std::map<std::string, std::string>>
Args::parse_args(int argc, char *argv[]) {

    auto opt_table = KW::options_table();

    if((argc - 1) % 2 != 0) {
        /* not according one-to-one key-value pair */
        // TODO: more hint
        return std::nullopt;
    }

    // TODO: const it
    std::map<std::string, std::string> user_ops;
    for (int i = 1; i < argc - 1; i += 2) {
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
            // more hint
            return std::nullopt;
        }

    }
    return user_ops;
}


