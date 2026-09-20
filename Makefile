CC      = clang
CFLAGS  = -Wall -Wextra -g -Isrc
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET  = chip8

SRC = $(wildcard src/*.c src/utils/*.c)

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean