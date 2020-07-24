CC = gcc
CFLAGS =

all: emo

emo: emo.c emo.h
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -fv emo
