# st version
VERSION = 0.3

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I. -I/usr/include
LIBS = -lutil -L${CC_ROOT}/lib -L${CC_ROOT}/usr/lib -lSDL2 \
-Wl,-Bstatic -lSDL2_ttf -lfreetype -lpng12 \
-Wl,-Bdynamic -lstdc++ -lz -lm -lc \
-Wl,-rpath-link,${CC_ROOT}/lib,-rpath-link,${CC_ROOT}/usr/lib 

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS += -g -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS} `sdl-config --cflags`
CXXFLAGS += -g -std=c++98 -Wpedantic -Wall -Os ${INCS} ${CPPFLAGS} `sdl-config --cflags`
LDFLAGS += -g ${LIBS}
#-Wl,-rpath-link,${CC_ROOT}/lib,-rpath-link,${CC_ROOT}/usr/lib

# compiler and linker
ifeq ($(CC_ROOT),)
CC = gcc
CXX = g++
else
CC  = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++
endif
