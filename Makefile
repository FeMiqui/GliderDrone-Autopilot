CC = gcc
CFLAGS = -Wall
LIBS = -lm
TARGET = autopilot
SRCS := $(wildcard *.c)

all: $(TARGET)

debug: CFLAGS += -g -O0
debug: $(TARGET)

release: CFLAGS += -O3
release: $(TARGET)


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(TARGET)
