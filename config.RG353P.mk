#
#	Simple Terminal config for RG353P Recalbox
#		based on RG350 ver https://github.com/jamesofarrell/st-sdl


VERSION = 0.3

# Customize below to fit your system
CC = /home/ark/recalbox-rg353x/output/host/bin/aarch64-buildroot-linux-gnu-gcc
CXX = /home/ark/recalbox-rg353x/output/host/bin/aarch64-buildroot-linux-gnu-g++
SYSROOT = $(shell ${CC} --print-sysroot)
SDL_CONFIG := $(shell $(CC) -print-sysroot)/usr/bin/sdl2-config

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I. -I${SYSROOT}/usr/include
LIBS = -lutil -L${SYSROOT}/lib -L${SYSROOT}/usr/lib $(shell $(SDL_CONFIG) --libs) \
-Wl,-Bstatic -lSDL2_ttf -lfreetype -lpng16 \
-Wl,-Bdynamic -lstdc++ -lz -lm -lc -lharfbuzz \
-Wl,-rpath-link,${SYSROOT}/lib,-rpath-link,${SYSROOT}/usr/lib 

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS += -s -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS} $(shell $(SDL_CONFIG) --cflags)
CXXFLAGS += -s -std=c++98 -Wpedantic -Wall -Os ${INCS} ${CPPFLAGS} $(shell $(SDL_CONFIG) --cflags)
LDFLAGS += -s ${LIBS}
