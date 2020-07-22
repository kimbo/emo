#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "emo.h"

#define DEBUG 1
#define NEXT(f, c) *c = getc(f); if (*c == EOF) { goto cleanup; }
#define SKIPWHITESPACE(f, c) while (1) {\
	*c = getc(f);\
	if (*c == EOF) {\
		goto cleanup;\
	}\
	if (isspace(*c)) {\
		continue;\
	}\
}
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

void advance(FILE *f) {
	c = getc(f);
	buf[charCount++] = c;
	if (c == '\n') {
		currentLine++;
	}
}

void saveCurrentToken() {
	token *t = newToken();
	t->position = currentLine;
	t->tokentype = currentTokenType;
	memcpy(t->value, &buf[0], charCount);
	tokens[tokenNumber++] = t;
}

void init() {
	currentLine = 1;
	charCount = 0;
	tokenNumber = 0;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage_and_quit(1);
	}
	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		perror("fopen");
		exit(1);
	}
	debug("beginning to read file");

	while (c != EOF) {
		c = getc(f);
		if (isdigit(c)) {
			// read number
		} else if (isalpha(c)) {
			// letter
			switch (c) {
			case 'f':
				// check for fun(ction)
				NEXT(f, &c);
				if (c == 'u') {
					NEXT(f, &c);
					if (c == 'n') {
						NEXT(f, &c);
						if (isspace(c)) {
							// fun(ction)
							break;
						} else {

						}
					} else {

					}
				} else {

				}
				break;
			case 'i':
				// check for if
				c = getc(f);
				if (c == 'f') {
					c = getc(f);
				}
				break;
			case 'n':
				// check for num(ber)
				c = getc(f);
				break;
			case 's':
				// check for str(ing)
				c = getc(f);

				break;
			case 'w':
				// check for whi(le)
				break;
			case ';':
				break;
			default:
				// identifier?
				break;
			}
		} else {
			// something else
			token *t = findOneCharToken(c);
			if (t != NULL) {
				t->position = position;
			}
			switch (c) {
			case '#':
				// read comment
				while (1) {

				}
				break;
			case EOF:
				goto cleanup;
				break;
			default:
				break;
			}
		}
	}
	cleanup:
		debug("cleaning up");
		fclose(f);
}
