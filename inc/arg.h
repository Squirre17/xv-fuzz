#ifndef __ARG_H__
#define __ARG_H__
/*
    deal with argv ,supply argv configure to every modules
*/
#include "common.h"
#include <map>
#include <string>
#include <optional>
#include <list>
#include <vector>
#include <exception>

// TODO: make it only internal use
struct Option {
    std::string key, val, des; /* description */

    explicit Option(
        const std::string &key, 
        // const std::string &val,
        const std::string &des
    );
};

/* keyword for arguments handle */
namespace KW { 
    /* input corpus */
    static const std::string INPUT        = "-i";
    /* output sample */
    static const std::string OUTPUT       = "-o";
    /* max timeout span */
    static const std::string TIMEOUT_SPAN = "-t";

    const std::vector<Option> options_table();
}

class Args{
public:
    Args &parse_args(int argc, char *argv[]);
    Args &process();

private:
    std::string fuzzee; /* fuzzee name */
    std::map<std::string, std::string> user_ops;




};

#endif // __ARG_H__