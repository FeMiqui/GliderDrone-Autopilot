CC = gcc
CFLAGS = -Wall
TARGET = autopilot
SRCS := $(wildcard *.c)

all: $(TARGET)

debug: CFLAGS += -g -O0
debug: $(TARGET)

release: CFLAGS += -O3
release: $(TARGET)


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
