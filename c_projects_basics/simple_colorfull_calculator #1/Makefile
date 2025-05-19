# Makefile
CC      := gcc
PKG_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
PKG_LIBS   := $(shell pkg-config --libs gtk+-3.0)
CFLAGS  := -Wall -g $(PKG_CFLAGS)
LDFLAGS := $(PKG_LIBS)

SRCS    := main.c calculator.c
OBJS    := $(SRCS:.c=.o)
TARGET  := kalkulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
