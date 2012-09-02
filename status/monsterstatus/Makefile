NAME    = monsterstatus

PREFIX ?= /usr/local
BINDIR ?= ${PREFIX}/bin

INCS = -I. -I/usr/include
LIBS = -L/usr/lib

CFLAGS  = -std=c99 -pedantic -Wall -Wextra ${INCS}
LDFLAGS = ${LIBS}

CC 	 = cc
EXEC = ${NAME}

SRC = ${NAME}.c
OBJ = ${SRC:.c=.o}

all: CFLAGS += -Os -march=native
all: LDFLAGS += -s
all: options ${NAME}

debug: CFLAGS += -O0 -ggdb
debug: options ${NAME}

options:
	@echo ${NAME} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

.o:
	@echo CC -o $@
	@${CC} -o $@ $< ${LDFLAGS}

clean:
	@echo cleaning
	@rm -fv ${NAME} ${OBJ}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@install -Dm755 ${NAME} ${DESTDIR}${PREFIX}/bin/${NAME}

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/${NAME}

.PHONY: all options clean install uninstall
