CC     = clang
CFLAGS = -Wall -Wextra -g
TARGET = chip8

all:
	$(CC) $(CFLAGS) src/*.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean