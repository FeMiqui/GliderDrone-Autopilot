CC = gcc
CFLAGS = -Wall
TARGET = autopilot

debug: CFLAGS += -g -O0
debug: $(TARGET)

release: CFLAGS += -O3
release: $(TARGET)

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
