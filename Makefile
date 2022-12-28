# required library (ncurses)

CC?=cc
LIBS=-lncurses

SRC_DIR=./src
EXE_DIR=.

EXES = ${EXE_DIR}/matrix_game

${EXE_DIR}/%: %.o
	${CC} -o $@ $< ${LIBS}

%.o: ${SRC_DIR}/%.c
	${CC} -o $@ -c $<

all:    ${EXES}

clean:
	@rm -f ${EXES}
