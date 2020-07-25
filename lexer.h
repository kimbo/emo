#ifndef EMO_LEXER_H
#define EMO_LEXER_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define TOKENBUFSIZE 1024
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

token *newToken();

void deinitToken(token *t);

typedef struct tokenTypeAndString {
    enum TOKENTYPE tokenType;
    char *string;
} tokenTypeAndString;

char *tokenToString(enum TOKENTYPE tokenType);

typedef struct lexer {
    FILE *f;
    token *tokens[MAXNUMBEROFTOKENS];
    int currentLine;
    int charCount;
    int tokenNumber;
    enum TOKENTYPE currentTokenType;
    int c;
    char buf[TOKENBUFSIZE];
} lexer;

int lexFile(char *fname, lexer *lex);

void cleanupTokens(lexer *lex);

#endif /* EMO_LEXER_H */