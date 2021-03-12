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

static int parse_flags_sanitize_input(flag_container_t *flags, int count);

/**
 * @brief Check that input flags are garbage
 *
 * @param[in]   flags   array of flags
 * @param[in]   count   number of flags
 *
 * @return non-zero if flags are invalid
 *         0 if flags are valid
 */
static int parse_flags_sanitize_input(flag_container_t *flags, int count)
{
    flag_container_t *curr = NULL; 

    if (flags == NULL) {
        return -1;
    }

    for (int i = 0; i < count; ++i) {
        curr = &flags[i];
        if (curr->message == NULL
            || curr->flag == NULL
            || curr->storage == NULL
            || curr->parser == NULL) {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief split a flag on the '=' character for parsing
 *
 * @param[in]  input      user input 
 * @param[out] flag_str   flag output 
 * @param[out] value_str  value entered by user before parse
 *
 * @return non-zero on error;
 */
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

/**
 * @brief Initialize the parse flags structure
 *
 * @param[in]  flags         array of flags
 * @param[in]  flag_count    number of flags
 * @param[out] pf_inst       parse flag instance
 *
 * @return non-zero on error
 *         zero on success
 */
int parse_flags_init(flag_container_t *flags, int flag_count, parse_flags_t *pf_inst)
{
    if (parse_flags_sanitize_input(flags, flag_count) != 0) {
        return -1;
    }

    pf_inst->total_flags = flag_count;
    pf_inst->flags = flags;
    pf_inst->message = NULL; 
    return 0;
}

/**
 * @brief add a message to show with help 
 * 
 * @param[in] pf_inst   instance of parse flags
 * @param[in] message   message to add
 */
void parse_flags_add_message(parse_flags_t *pf_inst, const char *message)
{
    pf_inst->message = message;
}

/**
 * @brief show help text associated with this instance
 *
 * @param [in]  pf_inst parse flags instance
 */
void parse_flags_show_help(parse_flags_t *pf_inst)
{
    size_t max_padding = 0;
    size_t flag_size   = 0;
    flag_container_t *curr = NULL;

    if (pf_inst == NULL) {
        return;
    }
    
    if (pf_inst->message != NULL) {
        printf("%s\n", pf_inst->message);
    }
    
    /* get the length of the longest flag */
    for (int i = 0; i < pf_inst->total_flags; ++i) {
        curr = &pf_inst->flags[i];
        flag_size = strlen(curr->message);
        if (flag_size > max_padding) {
            max_padding = flag_size;
        }
    }

    for (int i = 0; i < pf_inst->total_flags; ++i) {
        curr = &pf_inst->flags[i];
        printf("--%*s%s\n", -((int)max_padding + 5), curr->flag, curr->message);        
    }
}

/**
 * @brief parse user input based on parse flags instance
 *
 * @param[in]   argc    number of user provided arguments
 * @param[in]   argv    array of user provided arguments
 * @param[in]   pf_inst instance of parse flags to use
 *
 * @return non-zero on error
 *         0 on success
 */
int parse_flags_parse(int argc, char *argv[], parse_flags_t *pf_inst)
{
    int   res       = 0;  
    char *flag_str  = NULL;
    char *value_str = NULL;
    flag_container_t *curr = NULL;
   
    if (argc == 1) {
        parse_flags_show_help(pf_inst);
        exit(1);
    }

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


// Library provided parsing functions
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
