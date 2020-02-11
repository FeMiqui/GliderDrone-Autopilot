CC = gcc
CFLAGS = -Wall
TARGET = autopilot
SRCS = nmea.c gps.c $(TARGET).c

debug: CFLAGS += -g -O0
debug: $(TARGET)

release: CFLAGS += -O3
release: $(TARGET)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
