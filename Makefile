CC = gcc
CFLAGS = -g -Wall -Werror
TESTCFLAGS = -fprofile-arcs -ftest-coverage

SRCFILES = emo.c lexer.c

.PHONY = clean check fmt coverage

all: emo

emo: $(SRCFILES) lexer.h
	$(CC) $(CFLAGS) -o $@ $(SRCFILES)

clean:
	@rm -f emo
	@rm -f *.o
	@rm -f *.gcov
	@rm -f *.gcno
	@rm -f *.gcda

fmt:
	clang-format -i -style=file $(SRCFILES)

check:
	$(MAKE) clean
	$(MAKE)
	./tests/run.sh

coverage: CFLAGS := $(CFLAGS) $(TESTCFLAGS)
coverage:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS)"
	./tests/run.sh
	gcov $(SRCFILES)
	@for f in $(SRCFILES); do \
		printf '\nCoverage details for %s:\n' "$$f"; \
		cat "$$f.gcov"; \
	done
