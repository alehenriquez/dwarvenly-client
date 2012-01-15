ROOT_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

CC = clang
CFLAGS = -fPIC -std=c99 -O2 -W -Wall -Wextra -Werror \
 -fno-strict-aliasing -DNDEBUG -pipe							 \
 -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
 -I/usr/local/include -L/usr/local/lib

CFILES = $(wildcard *.c)

all: client

client:
	$(CC) $(CFLAGS) $(CFILES) /usr/local/lib/libglfw.a -lGL -lX11 -lXrandr -o build/client

clean:
	rm -rf build
	mkdir build

