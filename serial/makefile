prog := bin/sha1_hack
CC := gcc
CFLAGS := -Wall -std=c99

.PHONY: all clean dir

all: $(prog)

$(prog): build/main.o build/key_generator.o
	$(CC) $(CFLAGS) build/main.o build/key_generator.o -lcrypto -lm -o $(prog)
build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o
build/key_generator.o: src/key_generator.c 
	$(CC) $(CFLAGS) -c src/key_generator.c -o build/key_generator.o

clean:
	@rm -rf build/*.o

dir:
	@mkdir build
	@mkdir bin