#ifndef EMO_H
#define EMO_H

#include <ctype.h>
#include <string.h>

#define TOKENBUFSIZE 1024
#define MAXTOKENLEN 1024
#define MAXNUMBEROFTOKENS 65536

static const int POSITION_UNSPEC = -1;

enum TOKENTYPE {
    /* token of undefined length */
    IDENTIFIER,
    NUMBER,
    RAWSTRING,
    COMMENT,
    EMOJI,

    /* tokens with three characters */
    STR,
    NUM,
    FUN,
    RET,
    WHI,

    /* tokens with two characters */
    IF,

    /* one character tokens */
    OPENPAREN,
    CLOSEPAREN,
    OPENBRACE,
    CLOSEBRACE,
    COMMA,
    EQUALS,
    SEMICOLON,
    TAB,
    SPACE,
    NEWLINE,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,

    /* unknown */
    UNKNOWN,
};

struct token {
    enum TOKENTYPE tokentype;
    int lineno;
    char *value;
};
typedef struct token token;

token *newToken() {
    token *t = malloc(sizeof(token *));
    t->tokentype = UNKNOWN;
    t->lineno = POSITION_UNSPEC;
    t->value = malloc(sizeof(char) * TOKENBUFSIZE);
    return t;
}

void deinitToken(token *t) {
    if (t->value != NULL) {
        free(t->value);
    }
    free(t);
}

static const token threeCharTokens[] = {
    {NUM, POSITION_UNSPEC, "num"},
    {FUN, POSITION_UNSPEC, "fun"},
    {STR, POSITION_UNSPEC, "str"},
    {RET, POSITION_UNSPEC, "ret"},
    {WHI, POSITION_UNSPEC, "whi"},
};

static const token oneCharTokens[] = {
    /* parentheses */
    {OPENPAREN, POSITION_UNSPEC, "("},
    {CLOSEPAREN, POSITION_UNSPEC, ")"},

    /* curly braces */
    {OPENBRACE, POSITION_UNSPEC, "{"},
    {CLOSEBRACE, POSITION_UNSPEC, "}"},

    /* whitespace */
    {NEWLINE, POSITION_UNSPEC, "\n"},
    {TAB, POSITION_UNSPEC, "\t"},
    {SPACE, POSITION_UNSPEC, " "},

    /* math symbols */
    {ADD, POSITION_UNSPEC, "+"},
    {SUBTRACT, POSITION_UNSPEC, "-"},
    {MULTIPLY, POSITION_UNSPEC, "*"},
    {DIVIDE, POSITION_UNSPEC, "/"},
    {MODULO, POSITION_UNSPEC, "%"},

    /* other */
    {EQUALS, POSITION_UNSPEC, ","},
    {COMMA, POSITION_UNSPEC, ","},
    {SEMICOLON, POSITION_UNSPEC, ";"},
};

#endif /* EMO_H */