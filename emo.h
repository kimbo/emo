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

    /* tokens with three characters */
    STR,
    NUM,
    FUN,
    RET,
    WHI,

    /* tokens with two characters */
    IF,

    /* one character tokens */
    SINGLEQUOTE,
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
    int position;
    char *value;
};
typedef struct token token;

token *newToken() {
    token *t = malloc(sizeof(token *));
    t->tokentype = UNKNOWN;
    t->position = POSITION_UNSPEC;
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
    {SINGLEQUOTE, POSITION_UNSPEC, "'"},
};

token *findTokenWithGivenLength(char *s, int len) {

}

token *findThreeCharToken(char *s) {
    token *start = threeCharTokens;
    token *end = start + sizeof(threeCharTokens) / sizeof(threeCharTokens[0]);
    for (; start < end; start++) {
        if (strstr(s, start->value) == 0) {
            return start;
        }
    }
    return NULL;
}

token *findOneCharToken(char c) {
    token *start = oneCharTokens;
    token *end = start + sizeof(oneCharTokens)/sizeof(oneCharTokens[0]);
    for (; start < end; start++) {
        if (c == start->value) {
            return start;
        }
    }
    return NULL;
}

char skipWhitespace(FILE *f) {
    char c;
    while (1) {
        c = getc(f);
        if (!isspace(c)) {
            return c;
        }
    }
}

token *readNextToken(FILE *f) {
    char c;
    char buf[TOKENBUFSIZE];
    memset(buf, 0, TOKENBUFSIZE);
    int pos = 0;
    while (1) {
        c = skipWhitespace(f);
        if (c == EOF) {
            return NULL;
        }

        token *t = findOneCharToken(c);
        if (t != NULL && pos == 0) {
            return t;
        } else if (t != NULL && pos > 0) {
            putc(c, f);
            t = newToken();
            t->position = pos;
            memcpy(t->value, );
        }

        buf[pos++] = c;

        if (pos == 3) {

        }
    }
}

#endif /* EMO_H */