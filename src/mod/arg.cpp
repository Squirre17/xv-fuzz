
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
    @brief  : parse cmd line args for fuzzer
*/
Args &Args::parse_fuzzer_args(int argc, char *argv[]) {

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
namespace {
    static const std::string CC     = "clang";
    static const std::string XV_CC  = "xv-clang";
    static const std::string CXX    = "clang++";
    static const std::string XV_CXX = "xv-clang++";
}
// using std::filesystem::path;
namespace fs = std::filesystem;
fs::path get_root() {
    char *p = getenv("XV_PATH");
    if (p == NULL) {
        WARN("Plz set XV_PATH to root of xv-fuzz");
        exit(1);
    }

    fs::path root(p);
    if(not fs::exists(root)){
        WARN("XV_PATH not exist");
        exit(1);
    }

    return root;
}
fs::path find_obj(string_view s) {
    fs::path root = get_root();

    fs::path obj = root.append("objs")
                       .append("src")
                       .append("mod")
                       .append(s.data());

    DBG("obj : %s", obj.c_str()); 
    assert(fs::exists(obj));
    return obj;
}
Args &Args::parse_clang_args(int argc, char *argv[]) {
    // xv-clang -o main main.c -g 
    std::vector<std::string> params;
    if (argc < 4) {
        WARN("the arguments is too few");
        exit(1);
    }

    std::string name = std::string(argv[0]);
    size_t pos;
    if(( pos = name.rfind("/")) != std::string::npos) {
        name = name.substr(pos + 1);
    }
    std::cout << "[DBG] : name is " << name << std::endl;

    if(name == XV_CC) {
        params.push_back(CC);
    }else if (name == XV_CXX){
        params.push_back(CXX);
    }else {
        WARN("program must in { %s, %s}", XV_CC.c_str(), XV_CXX.c_str());
        exit(1);
    }
    
    params.push_back(std::string("-Xclang"));
    params.push_back(std::string(
        find_obj("xv-llvm-pass.so").c_str()
    ));

    size_t idx = 1;
    while(idx < argc) {
        params.push_back(std::string(argv[idx++]));
    }

    this->params = params;
    
    return *this;
}
Args &Args::process() {
    // TODO: need path lib to open a file 
    // TODO: check 32 / 64
    // TODO: check AFL_USE_ASAN
    return *this;
}

Args &Args::show_params() {
    for(const auto &par : this->params) {
        std::cout << "[DBG] : " << par.c_str() << std::endl;
    }
    return *this;
}


void Args::exec(){
    u32 len = params.size();
    const char **argv {(const char **)new char *[len]};

    for (size_t i = 0; i < len; i++) {
        argv[i] = params[i].c_str();
    }

    execvp(argv[0], (char **)argv);

    FATAL("execvp failed");
}