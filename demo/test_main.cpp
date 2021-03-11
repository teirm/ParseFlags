// Demo program for ParseFlags Class
//
// @author teirm
// @date   2021-03-11


#include "my_parsers.hpp"
//TODO is there a way to clean up these relative paths
#include "../src/cpp/parse_flags.hpp"

#include <cstdio>

using namespace parse_flags;

// a custom parser added into the parse flags namepsace
int parse_flags::parse_value(const char *value_str, const char *&storage) {
    storage = value_str;
    return 0;
}

int main(int argc, char *argv[])
{
    int a = 0;
    int b = 0;
    int c = 0;
    double d = 0.0;
    bool t = false;
    const char *s = nullptr;
    ParseFlags parser;
    
    parser.add_message("A demo program for the ParseFlags class");
    parser.add_flag("wombat-fish-chickens-moo", a, "int for a");
    parser.add_flag("b", b, "int for b");
    parser.add_flag("c", c, "int for c");
    parser.add_flag("d", d, "double for d");
    parser.add_flag("t", t, "bool for t");
    parser.add_flag("s", s, "string for s");

    parser.parse_args(argc, argv);
    printf("a = %d\nb = %d\nc = %d\nd = %lf\nt = %d\ns=%s\n",
            a, b, c, d, t, s);
    return 0;
}
