CC      = clang
CFLAGS  = -Wall -Wextra -g
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET  = chip8

all:
	$(CC) $(CFLAGS) src/*.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean