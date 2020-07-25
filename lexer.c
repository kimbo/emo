#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

void fatal(const char *msg, lexer *lex)
{
        fprintf(stderr, "[FATAL] line %d:%d: %s", lex->currentLine,
                lex->charCount, msg);
        exit(EXIT_FAILURE);
}

token *newToken()
{
        token *t = malloc(sizeof(token *));
        t->tokentype = UNKNOWN;
        t->lineno = LINENO_UNSPEC;
        t->value = malloc(sizeof(char) * TOKENBUFSIZE);
        return t;
}

void deinitToken(token *t)
{
        if (t->value != NULL) {
                free(t->value);
        }
        free(t);
}

tokenTypeAndString tokenTypesAndStrings[] = {
    {IDENTIFIER, "IDENTIFIER"},
    {NUMBER, "NUMBER"},
    {RAWSTRING, "RAWSTRING"},
    {COMMENT, "COMMENT"},
    {EMOJI, "EMOJI"},
    {KW_STR, "KW_STR"},
    {KW_NUM, "KW_NUM"},
    {KW_FUN, "KW_FUN"},
    {KW_RET, "KW_RET"},
    {KW_WHI, "KW_WHI"},
    {KW_IF, "KW_IF"},
    {OPENPAREN, "OPENPAREN"},
    {CLOSEPAREN, "CLOSEPAREN"},
    {OPENBRACE, "OPENBRACE"},
    {CLOSEBRACE, "CLOSEBRACE"},
    {COMMA, "COMMA"},
    {EQUALS, "EQUALS"},
    {SEMICOLON, "SEMICOLON"},
    {TAB, "TAB"},
    {SPACE, "SPACE"},
    {NEWLINE, "NEWLINE"},
    {ADD, "ADD"},
    {SUBTRACT, "SUBTRACT"},
    {MULTIPLY, "MULTIPLY"},
    {DIVIDE, "DIVIDE"},
    {MODULO, "MODULO"},
    {LESSTHAN, "LESSTHAN"},
    {GREATERTHAN, "GREATERTHAN"},
    {TOKEN_EOF, "TOKEN_EOF"},
    {UNKNOWN, "UNKNOWN"},
};

char *tokenToString(enum TOKENTYPE tokenType)
{
        tokenTypeAndString *start = tokenTypesAndStrings;
        tokenTypeAndString *end = start + sizeof(tokenTypesAndStrings) /
                                              sizeof(tokenTypesAndStrings[0]);
        for (; start < end; start++) {
                if (start->tokenType == tokenType) {
                        return start->string;
                }
        }
        return NULL;
}

void back(lexer *lex)
{
        if (lex->c != EOF && ungetc(lex->c, lex->f) == -1) {
                perror("ungetc");
                exit(EXIT_FAILURE);
        }
        lex->charCount--;
        lex->buf[lex->charCount] = '\0';
}

void forward(lexer *lex)
{
        lex->c = fgetc(lex->f);
        lex->buf[lex->charCount++] = lex->c;
        if (lex->c == '\n') {
                lex->currentLine++;
        }
}

void saveCurrentToken(lexer *lex)
{
        token *t = newToken();
        t->lineno = lex->currentLine;
        t->tokentype = lex->currentTokenType;
        memcpy(t->value, lex->buf, lex->charCount);
        lex->tokens[lex->tokenNumber++] = t;
        fprintf(stderr, "%s on line %d: %s\n",
                tokenToString(lex->currentTokenType), t->lineno, t->value);
}

void initLexer(char *fname, lexer *lex)
{
        lex->currentLine = 1;
        lex->charCount = 0;
        lex->tokenNumber = 0;
        lex->currentTokenType = UNKNOWN;
        lex->f = fopen(fname, "r");
        if (lex->f == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
        }
}

void lexComment(lexer *lex)
{
        while (lex->c != '\n' && lex->c != EOF) {
                forward(lex);
        }
        back(lex);
        lex->currentTokenType = COMMENT;
        if (lex->c == '\n') {
                lex->currentLine--;
                saveCurrentToken(lex);
                lex->currentLine++;
        }
        else {
                saveCurrentToken(lex);
        }
}

void lexRawString(lexer *lex)
{
        do {
                forward(lex);
        } while (lex->c != '\n' && lex->c != EOF && lex->c != '"');
        if (lex->c == '\n') {
                fatal("unterminated string caused by newline", lex);
        }
        if (lex->c == EOF) {
                fatal("unterminated string caused by end of file", lex);
        }
        lex->currentTokenType = RAWSTRING;
        saveCurrentToken(lex);
}

void lexEmoji(lexer *lex)
{
        do {
                forward(lex);
        } while (isalnum(lex->c));
        if (lex->c == '\n') {
                fatal("unterminated emoji caused by newline", lex);
        }
        if (lex->c == EOF) {
                fatal("unterminated emoji caused by end of file", lex);
        }
        if (lex->c != ':') {
                fatal("unterminated emoji caused by invalid character", lex);
        }
        lex->currentTokenType = EMOJI;
        saveCurrentToken(lex);
}

void lexIdentifierOrKeyword(lexer *lex)
{
        while (isalnum(lex->c) || lex->c == '_') {
                forward(lex);
        }
        back(lex);
        if (strcmp(lex->buf, "fun") == 0) {
                lex->currentTokenType = KW_FUN;
        }
        else if (strcmp(lex->buf, "str") == 0) {
                lex->currentTokenType = KW_STR;
        }
        else if (strcmp(lex->buf, "if") == 0) {
                lex->currentTokenType = KW_IF;
        }
        else if (strcmp(lex->buf, "whi") == 0) {
                lex->currentTokenType = KW_WHI;
        }
        else if (strcmp(lex->buf, "num") == 0) {
                lex->currentTokenType = KW_NUM;
        }
        else if (strcmp(lex->buf, "ret") == 0) {
                lex->currentTokenType = KW_RET;
        }
        else {
                lex->currentTokenType = IDENTIFIER;
        }
        if (lex->c == '\n') {
                lex->currentLine--;
                saveCurrentToken(lex);
                lex->currentLine++;
        }
        else {
                saveCurrentToken(lex);
        }
}

int lexFile(char *fname, lexer *lex)
{
        initLexer(fname, lex);
        while (lex->c != EOF) {
                forward(lex);
                if (lex->c == EOF) {
                        break;
                }
                switch (lex->c) {
                case '(':
                        lex->currentTokenType = OPENPAREN;
                        saveCurrentToken(lex);
                        break;
                case ')':
                        lex->currentTokenType = CLOSEPAREN;
                        saveCurrentToken(lex);
                        break;
                case '{':
                        lex->currentTokenType = OPENBRACE;
                        saveCurrentToken(lex);
                        break;
                case '}':
                        lex->currentTokenType = CLOSEBRACE;
                        saveCurrentToken(lex);
                        break;
                case ',':
                        lex->currentTokenType = COMMA;
                        saveCurrentToken(lex);
                        break;
                case '=':
                        lex->currentTokenType = EQUALS;
                        saveCurrentToken(lex);
                        break;
                case '+':
                        lex->currentTokenType = ADD;
                        saveCurrentToken(lex);
                        break;
                case '-':
                        lex->currentTokenType = SUBTRACT;
                        saveCurrentToken(lex);
                        break;
                case '*':
                        lex->currentTokenType = MULTIPLY;
                        saveCurrentToken(lex);
                        break;
                case '/':
                        lex->currentTokenType = DIVIDE;
                        saveCurrentToken(lex);
                        break;
                case '%':
                        lex->currentTokenType = MODULO;
                        saveCurrentToken(lex);
                        break;
                case ';':
                        lex->currentTokenType = SEMICOLON;
                        saveCurrentToken(lex);
                        break;
                case '>':
                        lex->currentTokenType = GREATERTHAN;
                        saveCurrentToken(lex);
                        break;
                case '<':
                        lex->currentTokenType = LESSTHAN;
                        saveCurrentToken(lex);
                        break;

                /* skip whitespace */
                case '\t':
                case '\n':
                case ' ':
                        break;

                case '#':
                        lexComment(lex);
                        break;
                case '"':
                        lexRawString(lex);
                        break;
                case ':':
                        lexEmoji(lex);
                        break;
                default:
                        if (isalnum(lex->c) || lex->c == '_') {
                                lexIdentifierOrKeyword(lex);
                                break;
                        }
                        else {
                                lex->currentTokenType = UNKNOWN;
                                saveCurrentToken(lex);
                                break;
                        }
                }
                memset(lex->buf, 0, TOKENBUFSIZE);
                lex->charCount = 0;
                lex->currentTokenType = UNKNOWN;
        }
        lex->currentTokenType = TOKEN_EOF;
        // avoid printing garbage when we reach EOF
        lex->buf[0] = '\0';
        lex->charCount = 1;

        saveCurrentToken(lex);

        fclose(lex->f);
        return 0;
}

void cleanupTokens(lexer *lex)
{
        int i;
        for (i = 0; i < lex->tokenNumber; i++) {
                deinitToken(lex->tokens[i]);
        }
}