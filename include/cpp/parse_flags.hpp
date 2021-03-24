// A class to parse command to create flags and and parse command line
// input.

#ifndef PARSE_FLAGS_HPP
#define PARSE_FLAGS_HPP

#include <vector>
#include <cstdio>
#include <cassert>
#include <string>

namespace parse_flags {

const unsigned int PARSE_FLAGS_DEFAULT_PAD = 5;

enum class ParseFlagStatus : int {
    PARSE_FLAGS_SUCCESS   = 0,
    PARSE_FLAGS_BAD_PARSE = 1,
};

int parse_value(const char *value_str, int &storage); 
int parse_value(const char *value_str, double &storage);
int parse_value(const char *value_str, bool &storage);

// A type erased type to hold storage for arbitrary
// values and parse them appropriately
class flag_storage_t {
public:
    // Type erasure:
    // this is a template constructor -- the class is 
    // still a concrete type.
    //
    // Things to note: The pointer to the storage 
    // is cast to void to store in a generic manner
    //
    // A lambda capture is done to cast the type 
    // appropriately and call the overloaded parse_value
    // function.
    template<typename T>
    flag_storage_t(const char *flag, T& storage, const char *message):
        flag(flag), 
        message(message),
        repr_((void *)&storage),
        parse_([](const char *str, void *storage)->int {
                return parse_value(str, *(T*)storage);
                }) { }
    
    // single public function calls the internal parse function
    int parse(const char *str) { return parse_(str, repr_); }

    const char *flag;
    const char *message;
private:
    void *repr_;
    int (*parse_)(const char *str, void *storage);
};

class ParseFlags final {
public:
    ParseFlags():message_(nullptr) { } 
    ParseFlags(const ParseFlags &pf) = delete;
    ParseFlags& operator=(const ParseFlags &pf) = delete;
    
    template<typename T> 
    void add_flag(const char *flag, T &storage, const char *message);
    void add_message(const char *message) { message_ = message; };
    int parse_args(int argc, char *argv[]);  
private:

    const char *message_;
    std::vector<flag_storage_t> flags_;
    void show_help();
    
    int parse_flag(char *input, char **flag, char **value_str);

    bool is_help(const char *input);
};

// @brief add a flag associated with some storage
// 
// @param[in]  flag      flag to add
// @param[out] storage   value to use as storage
// @param[in]  message   message associated with flag (optional)
template<typename T>
void ParseFlags::add_flag(const char *flag, T &storage, const char *message)
{
    flags_.emplace_back(flag, storage, message);
}

}

#endif 
