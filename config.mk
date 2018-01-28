# st version
VERSION = 0.3

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I. -I/usr/include
LIBS = -lc -lutil -L/lib -L/usr/lib

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS += -g -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS} `sdl-config --cflags`
LDFLAGS += -g ${LIBS} -lSDL2 -lSDL2_ttf 
#-Wl,-rpath-link,${CC_ROOT}/lib,-rpath-link,${CC_ROOT}/usr/lib

# compiler and linker
#CC = arm-linux-gnueabihf-gcc
CC = gcc
