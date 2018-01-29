# st - simple terminal
# See LICENSE file for copyright and license details.

include config.mk

SRC = st.c input_handler.cpp
OBJT = $(patsubst %.c,%.o,${SRC})
OBJ = $(patsubst %.cpp,%.o,${OBJT})

all: options st

options:
	@echo st build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CXXFLAGS = ${CXXFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "CXX      = ${CXX}"
	@echo "OBJ      = ${OBJ}"

config.h:
	cp config.def.h config.h

st.o: st.c input_handler.h
	@echo CC $<
	@${CC} -c st.c ${CFLAGS} $<

input_handler.o: input_handler.cpp input_handler.h
	@echo CXX $<
	@${CXX} -c ${CXXFLAGS} $<

st: st.o input_handler.o
	@echo CC -o st ${OBJ} ${LDFLAGS}
	@${CC} -o st ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f st ${OBJ} st-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p st-${VERSION}
	@cp -R LICENSE Makefile README config.mk config.def.h st.info st.1 ${SRC} st-${VERSION}
	@tar -cf st-${VERSION}.tar st-${VERSION}
	@gzip st-${VERSION}.tar
	@rm -rf st-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f st ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/st
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < st.1 > ${DESTDIR}${MANPREFIX}/man1/st.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/st.1
	@echo If things do not seem to work, be sure that there is no \
		floating st terminfo in the .terminfo directory in your home dir.
	@tic -s st.info

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/st.1

.PHONY: all options clean dist install uninstall
