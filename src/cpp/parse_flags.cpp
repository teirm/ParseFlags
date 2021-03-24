// Implementation of parse flags class


#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <cpp/parse_flags.hpp>

namespace parse_flags {
    
    int parse_value(const char *value_str, int &storage) 
    {
        assert(value_str != nullptr);
        int n = sscanf(value_str, "%d", &storage);
        return n == 1 ? 0 : -1; 
    }

    int parse_value(const char *value_str, double &storage) {
        assert(value_str != nullptr);
        int n = sscanf(value_str, "%lf", &storage);
        return n == 1 ? 0 : -1;
    }

    int parse_value(const char *value_str, bool &storage) {
        assert(value_str != nullptr);
        if (strncmp(value_str, "true", strlen("true")) == 0) {
            storage = true;
        } else if (strncmp(value_str, "false", strlen("false")) == 0) {
            storage = false;
        } else {
            // value str does not represent a bool
            return -1;
        }
        return 0;
    }
    
    // @brief Check if the given input is the help string
    //
    // @param[in]   input   input to check
    bool ParseFlags::is_help(const char *input) 
    {
        return (strncmp(input, "--help", strlen("--help")) == 0 ? true : false);
    }

    // @brief parse a string in the form of a flag and value and return
    // the flag and value
    //
    // @param[in]   input     the input string
    // @param[out]  flag      the flag found in the string
    // @param[out]  value_str the value associated with the flag as a string
    //
    // @return int  0 on success
    //              other on error
    //
    // @note flags have the form --<flag>=<value>. This function takes a 
    //       string in this form and returns flag and value
    //
    int ParseFlags::parse_flag(char *input, char **flag, char **value_str)
    {
        assert(input != nullptr);
        assert(flag != nullptr);
        assert(value_str != nullptr);

        // skip past the flag start "--"  
        for (int i = 0; i < 2; i++) { 
            if (*input != '-') {
                return 1;
            } 
            input++;
        }
        
        *flag = strsep(&input, "=");
        if (*flag == NULL) {
            return 1;
        }
        *value_str = input;  
        return 0;
    }
    
    // @brief parse the arguments given in the command line
    // based on the flags set
    //
    // @param[in]    argc   number of command line arguments
    // @param[in]    argv   command line argument vector
    // 
    // @return 0 on success 
    //         1 on error
    int ParseFlags::parse_args(int argc, char *argv[]) 
    {
        if ((size_t)argc - 1 > flags_.size()) {
            // more arguments than flags provided 
            // an error case -- ignore the program
            // name
            show_help();
            return 1;
        }

        char *flag = nullptr;
        char *value_str = nullptr;
        for (int i = 1; i < argc; ++i) {
            if (is_help(argv[i])) {
                show_help();
                break;
            }

            int res = parse_flag(argv[i], &flag, &value_str);
            if (res) {
                printf("error from parse_flag: %s\n", argv[i]);
                return res;
            }
            auto find_iter = std::find_if(flags_.begin(),
                         flags_.end(),
                         [flag](flag_storage_t &node) {
                            return !strncmp(flag, node.flag, strlen(node.flag));
                            }); 
            if (find_iter == flags_.end()) {
                printf("flag never set:\"%s\"\n", flag);
                // flag was never set 
                return 1;
            }
            // the storage type held as the second entry
            // knows how to parse itself.
            res = find_iter->parse(value_str);
            if (res) {
                return res;
            }
        }
        return 0;
    }

    // @brief show help message
    //
    // @note this is automatically triggered if the help 
    //        flag is found or if no args were provided
    void ParseFlags::show_help()
    {
        int flag_len = 0;
        int padding  = 0;

        if (message_ != nullptr) {
            printf("%s\n", message_);
        }
        
        std::for_each(flags_.begin(), flags_.end(), 
                      [&](flag_storage_t &e) {
                            flag_len = strlen(e.flag);
                            if (flag_len > padding) {
                                padding = flag_len;
                            }
                        });

        for (auto &e : flags_) {
            printf(" --%*s%s\n", 
                    -(padding+PARSE_FLAGS_DEFAULT_PAD), 
                    e.flag, e.message);
        }
        std::exit(1);
    }
}
