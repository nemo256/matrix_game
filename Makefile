# required library (ncurses)

CC?=cc
LIBS=-lncurses

SRC_DIR=./src
EXE_DIR=.

EXES = ${EXE_DIR}/matrix_game

all:    ${EXES}

${EXE_DIR}/%: %.o
	${CC} -o $@ $< ${LIBS}

%.o: ${SRC_DIR}/%.c
	${CC} -o $@ -c $<

install: all
	mkdir -p /usr/local/bin
	cp -f ${EXES} /usr/local/bin
	chmod 755 /usr/local/bin/${EXES}

uninstall:
	rm -f /usr/local/bin/${EXES}

clean:
	@rm -f ${EXES}
