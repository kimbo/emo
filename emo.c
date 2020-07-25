#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

const char *program_name = "emo";

void usage_and_quit(int code)
{
        fprintf(stderr, "Usage: %s <filename>\n", program_name);
        exit(code);
}

int main(int argc, char *argv[])
{
        if (argc < 2) {
                usage_and_quit(1);
        }
        lexer lex;
        int retval = lexFile(argv[1], &lex);
        (void)retval;
        cleanupTokens(&lex);
}
