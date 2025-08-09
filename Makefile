# slock - Simple command line soft locker
# Created by Rouvik Maji 2025

.PHONY: all prepare clean

CC := gcc

PROJECT_NAME := slock

BUILD_DIR = ./bin

# Uncomment to enable building with debug flags or use -DDEBUG=-ggdb to enable debug build from make cli
# DEBUG = -ggdb

CFLAGS = -Wall $(DEBUG)

all: prepare
	$(CC) main.c $(CFLAGS) -o $(BUILD_DIR)/$(PROJECT_NAME)

prepare:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)