DIRECTORY=$(shell pwd)
A = $(shell echo `ls -l /usr/bin/as` | cut -d '>' -f2 | cut -c 2-)
B = $(shell echo `ls -l /usr/bin/gcc` | cut -d '>' -f2 | cut -c 2-)

all: gcc as test

gcc: gcc.c
	gcc gcc.c -o gcc.out

as: as.c
	gcc as.c -o as.out

test: as_test.c
	echo "abc123" | sudo -S ln -Tfs $(DIRECTORY)/as.out /usr/bin/as
	echo "abc123" | sudo -S ln -Tfs $(DIRECTORY)/gcc.out /usr/bin/gcc
	gcc as_test.c -o test.out
	echo "abc123" | sudo -S ln -Tfs $(A) /usr/bin/as
	echo "abc123" | sudo -S ln -Tfs $(B) /usr/bin/gcc

clean:
	rm -f gcc.out
	rm -f as.out
	rm -f test.out
