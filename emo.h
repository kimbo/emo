#ifndef EMO_H
#define EMO_H

#include <ctype.h>
#include <string.h>

#define TOKENBUFSIZE 1024
#define MAXTOKENLEN 1024
#define MAXNUMBEROFTOKENS 65536

static const int LINENO_UNSPEC = -1;

enum TOKENTYPE {
    /* tokens of undefined length */
    IDENTIFIER,
    NUMBER,
    RAWSTRING,
    COMMENT,
    EMOJI,

    /* key word (KW) tokens */
    KW_STR,
    KW_NUM,
    KW_FUN,
    KW_RET,
    KW_WHI,
    KW_IF,

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
    LESSTHAN,
    GREATERTHAN,

    /* end of file */
    TOKEN_EOF,

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
    t->lineno = LINENO_UNSPEC;
    t->value = malloc(sizeof(char) * TOKENBUFSIZE);
    return t;
}

void deinitToken(token *t) {
    if (t->value != NULL) {
        free(t->value);
    }
    free(t);
}

typedef struct tokenTypeAndString {
    enum TOKENTYPE tokenType;
    char *string;
} tokenTypeAndString;

static tokenTypeAndString tokenTypesAndStrings[] = {
    {IDENTIFIER,"IDENTIFIER"},
    {NUMBER,"NUMBER"},
    {RAWSTRING,"RAWSTRING"},
    {COMMENT,"COMMENT"},
    {EMOJI,"EMOJI"},
    {KW_STR,"KW_STR"},
    {KW_NUM,"KW_NUM"},
    {KW_FUN,"KW_FUN"},
    {KW_RET,"KW_RET"},
    {KW_WHI,"KW_WHI"},
    {KW_IF,"KW_IF"},
    {OPENPAREN,"OPENPAREN"},
    {CLOSEPAREN,"CLOSEPAREN"},
    {OPENBRACE,"OPENBRACE"},
    {CLOSEBRACE,"CLOSEBRACE"},
    {COMMA,"COMMA"},
    {EQUALS,"EQUALS"},
    {SEMICOLON,"SEMICOLON"},
    {TAB,"TAB"},
    {SPACE,"SPACE"},
    {NEWLINE,"NEWLINE"},
    {ADD,"ADD"},
    {SUBTRACT,"SUBTRACT"},
    {MULTIPLY,"MULTIPLY"},
    {DIVIDE,"DIVIDE"},
    {MODULO,"MODULO"},
    {LESSTHAN,"LESSTHAN"},
    {GREATERTHAN,"GREATERTHAN"},
    {TOKEN_EOF,"TOKEN_EOF"},
    {UNKNOWN,"UNKNOWN"},
};

char *tokenToString(enum TOKENTYPE tokenType) {
    tokenTypeAndString *start = tokenTypesAndStrings;
    size_t s = sizeof(start);
    tokenTypeAndString *end = start + sizeof(tokenTypesAndStrings)/sizeof(tokenTypesAndStrings[0]);
    for (; start < end; start++) {
        if (start->tokenType == tokenType) {
            return start->string;
        }
    }
    return NULL;
}

#endif /* EMO_H */