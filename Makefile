all: main.c
	gcc -Wall main.c `pkg-config --cflags --libs gtk+-2.0` -o brightness

clean:
	rm -f brightness


.PHONY: clean
