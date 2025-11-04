DIRECTORY=$(shell pwd)
A = $(shell echo `ls -l /usr/bin/as` | cut -d '>' -f2 | cut -c 2-)
B = $(shell echo `ls -l /usr/bin/gcc` | cut -d '>' -f2 | cut -c 2-)

attack: attack_test.c
	gcc attack_test.c -o test.out

bit: 1bit_test.c
	gcc 1bit_test.c -o test.out

safe: 1bit_test.c
	echo "abc123" | sudo -S ln -Tfs $(DIRECTORY)/as_custom/as.out /usr/bin/as
	echo "abc123" | sudo -S ln -Tfs $(DIRECTORY)/as_custom/gcc.out /usr/bin/gcc
	gcc 1bit_test.c -o test.out
	echo "abc123" | sudo -S ln -Tfs $(A) /usr/bin/as
	echo "abc123" | sudo -S ln -Tfs $(B) /usr/bin/gcc

clean:
	rm -f test.out
