/* A c-style demo program for the parsing library 
 * 
 * @author teirm
 * @date   2021-03-11
 */

/* TODO: this relative path is a bit ugly... */
#include "../src/c/parse_flags.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int res = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int t = 0;
    double d = 0.0;
    size_t flag_count = 0;
    parse_flags_t pf_inst;
    flag_container_t my_flags[] = {{"a    int", "a", &a, parse_int},
                                   {"b    int", "b", &b, parse_int},
                                   {"c    int", "c", &c, parse_int},
                                   {"d double", "d", &d, parse_double},
                                   {"t   bool", "t", &t, parse_bool}}; 

    memset(&pf_inst, 0, sizeof(pf_inst));
    
    flag_count = sizeof(my_flags)/sizeof(my_flags[0]);
    res = parse_flags_init(my_flags, flag_count, &pf_inst);
    if (res) {
        printf("failure to parse flags: %d\n", res);
    }

    res = parse_flags_parse(argc, argv, &pf_inst);
    if (res) {
        parse_flags_show_help(&pf_inst);
    }
    
    printf("a=%d\nb=%d\nc=%d\nd=%lf\nt=%d\n",
            a, b, c, d, t);
    return 0;
}
