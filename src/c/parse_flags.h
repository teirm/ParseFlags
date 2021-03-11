// A C style implementation of the parse flags library

#include <string.h>

int parse_int(const char *str, void *storage);
int parse_double(const char *str, void *storage);
int parse_bool(const char *str, void *storage);

typedef int (*parser_func_t)(const char *str, void *storage); 

// A structure representing a flag and storage
typedef struct {
    const char      *message;        // a help message
    const char      *flag;           // the flag
    void            *storage;        // storage for the value
    parser_func_t    parser;
} flag_container_t;

// A structure for holding parse flags info
typedef struct {
    int total_flags;
    flag_container_t *flags;
} parse_flags_t;

// initialize the parse flags structure
void parse_flags_init(flag_container_t *flags, 
                      int flag_count, 
                      parse_flags_t *pf_inst);

void parse_flags_show_help(parse_flags_t *pf_inst);

int parse_flags_parse(int argc, char *argv[], parse_flags_t *pf_inst);

