CC     = gcc
CFLAGS = -g3 -std=gnu99 -pedantic -Wall
HWK    = /c/cs223/hw4

Calc: Calc.o stack.o
	${CC} ${CFLAGS} -o Calc Calc.o stack.o

Calc.o: Calc.c
	${CC} ${CFLAGS} -c Calc.c

stack.o: stack.c
	${CC} ${CFLAGS} -c stack.c

stacktest:  stacktest.o stack.o
	${CC} ${CFLAGS} -o stacktest stacktest.o stack.o

stacktest.o:  stacktest.c
		${CC} ${CFLAGS} -c stacktest.c

clean:
	rm -i *.o
