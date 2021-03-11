// A C style implementation of the parse flags 
// library
// For size curiousity

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse_flags.h"


static int parse_flags_split_flag(char *input, 
                                  char **flag_str, 
                                  char **value_str);

static int parse_flags_split_flag(char *input,
                                  char **flag_str,
                                  char **value_str)
{
    if (input == NULL || flag_str == NULL || value_str == NULL) {
        return -1;
    }
    
    // skip past the flag start "--"  
    for (int i = 0; i < 2; i++) { 
        if (*input != '-') {
            return 1;
        } 
        input++;
    }
    
    *flag_str = strsep(&input, "=");
    if (flag_str == NULL) {
        return -1;
    }
    *value_str = input;
    
    return 0;
}

void parse_flags_init(flag_container_t *flags, int flag_count, parse_flags_t *pf_inst)
{
    pf_inst->total_flags = flag_count;
    pf_inst->flags = flags;
}

void parse_flags_show_help(parse_flags_t *pf_inst)
{
    flag_container_t *curr = NULL;

    if (pf_inst == NULL) {
        return;
    }

    for (int i = 0; i < pf_inst->total_flags; ++i) {
        curr = &pf_inst->flags[i];
        printf("--%s\t\t\t%s\n", curr->flag, curr->message);        
    }
}

int parse_flags_parse(int argc, char *argv[], parse_flags_t *pf_inst)
{
    int   res       = 0;  
    char *flag_str  = NULL;
    char *value_str = NULL;
    flag_container_t *curr = NULL;
    
    for (int i = 1; i < argc; ++i) {
        res = parse_flags_split_flag(argv[i], &flag_str, &value_str);
        if (!strncmp(argv[i], "--help", strlen("--help"))) {
            parse_flags_show_help(pf_inst);
            exit(1);
        }

        if (res) {
            printf("error: unable to split %s\n", argv[i]);
            return res;
        }
        
        for (int j = 0; j < pf_inst->total_flags; ++j) {
            curr = &pf_inst->flags[j];
            if (!strncmp(flag_str, curr->flag, strlen(curr->flag))) {
                res = curr->parser(value_str, curr->storage);
                if (res) {
                    printf("error: unable to parse %s\n", argv[i]);
                }
            }
        }
    }
    return 0;
}

// Library provied parsing functions

int parse_int(const char *str, void *storage)
{
    int res = sscanf(str, "%d", (int *)storage);
    return res == 1 ? 0 : -1;
}

int parse_double(const char *str, void *storage)
{
    int res = sscanf(str, "%lf", (double *)storage);
    return res == 1 ? 0 : -1;
}

int parse_bool(const char *str, void *storage)
{
    if (!strncmp(str, "true", strlen("true"))) {
        *(int *)storage = 1;
        return 0;
    } else if (!strncmp(str, "false", strlen("false"))) {
        *(int *)storage = 0;
        return 0;
    } else {
        return 1;
    }
}
