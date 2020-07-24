#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "emo.h"

#define DEBUG 1

const char *program_name = "emo";

void usage_and_quit(int code)
{
	fprintf(stderr, "Usage: %s <filename>\n", program_name);
	exit(code);
}

void debug(const char *msg)
{
#if DEBUG
	fprintf(stderr, "[DEBUG]: ");
	fprintf(stderr, "%s\n", msg);
#endif
}

static int currentLine;
static enum TOKENTYPE currentTokenType;
static int charCount;
static char c;
static char buf[TOKENBUFSIZE];
static int tokenNumber;
static token *tokens[MAXNUMBEROFTOKENS];
static FILE *f;

void fatal(const char *msg) {
	fprintf(stderr, "[FATAL] line %d:%d: %s", currentLine, charCount, msg);
	exit(EXIT_FAILURE);
}

void back() {
	int i = ungetc(c, f);
	if (i == -1) {
		perror("ungetc");
		exit(EXIT_FAILURE);
	}
	charCount--;
	buf[charCount] = '\0';
}

void forward() {
	c = fgetc(f);
	buf[charCount++] = c;
	if (c == '\n') {
		currentLine++;
	}
}

void saveCurrentToken() {
	token *t = newToken();
	t->lineno = currentLine;
	t->tokentype = currentTokenType;
	memcpy(t->value, buf, charCount);
	tokens[tokenNumber++] = t;
	fprintf(stderr, "%s on line %d: %s\n", tokenToString(currentTokenType), t->lineno, t->value);
}

void init(char *fname) {
	currentLine = 1;
	charCount = 0;
	tokenNumber = 0;
	f = fopen(fname, "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
}

void lexComment() {
	while (c != '\n' && c != EOF) {
		forward();
	}
	back();
	currentTokenType = COMMENT;
	if (c == '\n') {
		currentLine--;
		saveCurrentToken();
		currentLine++;
	} else {
		saveCurrentToken();
	}
}

void lexRawString() {
	do {
		forward();
	} while (c != '\n' && c != EOF && c != '"');
	if (c == '\n') {
		fatal("unterminated string caused by newline");
	}
	if (c == EOF) {
		fatal("unterminated string caused by end of file");
	}
	currentTokenType = RAWSTRING;
	if (c == '\n') {
		currentLine--;
		saveCurrentToken();
		currentLine++;
	} else {
		saveCurrentToken();
	}
}

void lexEmoji() {
	do {
		forward();
	} while (isalnum(c));
	if (c == '\n') {
		fatal("unterminated emoji caused by newline");
	}
	if (c == EOF) {
		fatal("unterminated emoji caused by end of file");
	}
	if (c != ':') {
		fatal("unterminated emoji caused by invalid character");
	}
	currentTokenType = EMOJI;
	if (c == '\n') {
		currentLine--;
		saveCurrentToken();
		currentLine++;
	} else {
		saveCurrentToken();
	}
}

void lexIdentifierOrKeyword() {
	while (isalnum(c) || c == '_') {
		forward();
	}
	back();
	if (strcmp(buf, "fun") == 0) {
		currentTokenType = KW_FUN;
	} else if (strcmp(buf, "str") == 0) {
		currentTokenType = KW_STR;
	} else if (strcmp(buf, "if") == 0) {
		currentTokenType = KW_IF;
	} else if (strcmp(buf, "whi") == 0) {
		currentTokenType = KW_WHI;
	} else if (strcmp(buf, "num") == 0) {
		currentTokenType = KW_NUM;
	} else if (strcmp(buf, "ret") == 0) {
		currentTokenType = KW_RET;
	} else {
		currentTokenType = IDENTIFIER;
	}
	if (c == '\n') {
		currentLine--;
		saveCurrentToken();
		currentLine++;
	} else {
		saveCurrentToken();
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage_and_quit(1);
	}
	init(argv[1]);
	debug("lexing...");

	while (c != EOF) {
		forward();
		if (c == EOF) {
			break;
		}
		switch(c) {
        case '(':
			currentTokenType = OPENPAREN;
			saveCurrentToken();
			break;
		case ')':
			currentTokenType = CLOSEPAREN;
			saveCurrentToken();
			break;
		case '{':
			currentTokenType = OPENBRACE;
			saveCurrentToken();
			break;
		case '}':
			currentTokenType = CLOSEBRACE;
			saveCurrentToken();
			break;
		case ',':
			currentTokenType = COMMA;
			saveCurrentToken();
			break;
		case '=':
			currentTokenType = EQUALS;
			saveCurrentToken();
			break;
		case '+':
			currentTokenType = ADD;
			saveCurrentToken();
			break;
		case '-':
			currentTokenType = SUBTRACT;
			saveCurrentToken();
			break;
		case '*':
			currentTokenType = MULTIPLY;
			saveCurrentToken();
			break;
		case '/':
			currentTokenType = DIVIDE;
			saveCurrentToken();
			break;
		case '%':
			currentTokenType = MODULO;
			saveCurrentToken();
			break;
		case ';':
			currentTokenType = SEMICOLON;
			saveCurrentToken();
			break;
		case '>':
			currentTokenType = GREATERTHAN;
			saveCurrentToken();
			break;
		case '<':
			currentTokenType = LESSTHAN;
			saveCurrentToken();
			break;

		/* skip whitespace */
		case '\t':
		case '\n':
		case ' ':
			break;

		case '#':
			lexComment();
			break;
		case '"':
			lexRawString();
			break;
		case ':':
			lexEmoji();
			break;
		default:
			if (isalnum(c) || c == '_') {
				lexIdentifierOrKeyword();
				break;
			} else {
				currentTokenType = UNKNOWN;
				saveCurrentToken();
				break;
			}
		}
		memset(buf, 0, TOKENBUFSIZE);
		charCount = 0;
		currentTokenType = UNKNOWN;
	}
	currentTokenType = TOKEN_EOF;
	// avoid printing garbage
	buf[0] = '\0';
	charCount = 1;

	saveCurrentToken();

	cleanup:
		debug("cleaning up");
		fclose(f);
}
