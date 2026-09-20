CC     = clang
CFLAGS = -Wall -Wextra -g
TARGET = myChip8

all:
	$(CC) $(CFLAGS) src/*.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean