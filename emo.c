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
	fprintf(stderr, "[FATAL] line %s:%d: %s", currentLine, charCount, msg);
	exit(EXIT_FAILURE);
}

void back() {
	int i = putc(c, f);
	if (i == -1) {
		perror("putc");
		exit(EXIT_FAILURE);
	}
	currentLine--;
}

void forward() {
	c = getc(f);
	buf[charCount++] = c;
	if (c == '\n') {
		currentLine++;
	}
}

void saveCurrentToken() {
	token *t = newToken();
	t->lineno = currentLine;
	t->tokentype = currentTokenType;
	memcpy(t->value, &buf[0], charCount);
	tokens[tokenNumber++] = t;
}

void init(char *fname) {
	currentLine = 1;
	charCount = 0;
	tokenNumber = 0;
	FILE *f = fopen(fname, "r");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
}

void lexComment() {
	currentTokenType = COMMENT;
	while (c != '\n' && c != EOF) {
		forward();
	}
	// don't want to save newline or EOF in comment
	charCount--;
	currentLine--;
	saveCurrentToken();
	currentLine++;
}

void lexRawString() {
	while (c != '\n' && c != EOF && c != '"') {
		forward();
	}
	if (c == '\n') {
		fatal("unterminated string caused by newline");
	}
	if (c == EOF) {
		fatal("unterminated string caused by end of file");
	}
	currentTokenType = RAWSTRING;
	saveCurrentToken();
}

void lexEmoji() {
	while (c != '\n' && c != EOF && c != ':') {
		forward();
	}
	if (c == '\n') {
		fatal("unterminated emoji caused by newline");
	}
	if (c == EOF) {
		fatal("unterminated emoji caused by end of file");
	}
	currentTokenType = EMOJI;
	saveCurrentToken();
}

void lexIdentifierOrKeyword() {

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
	}
	currentTokenType = EOF;
	saveCurrentToken();
	cleanup:
		debug("cleaning up");
		fclose(f);
}
