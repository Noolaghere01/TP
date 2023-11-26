# Makefile

CC=gcc
CFLAGS=-Wall
TARGET=mon_programme

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)
