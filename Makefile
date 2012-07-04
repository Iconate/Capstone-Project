#Current 'make' runs independent of the objects as there are not yet correctly implemented. To run with objects use 'make compall' for a full compile.. This is be refined later.
#ggh:
#	gcc -Wall -o ggh ggh.c -lgsl -lgslcblas -lm

CC = gcc
EXEC = ggh
CFLAGS = -Wall -pg -lgsl -lgslcblas 
LIBS = -lm

#directories with the source code
SRCDIR1 = lib
SRCDIR2 = net
SRCDIR3 = test

#header files
HDRS = ${SRCDIR1}/constants.h

#Source file list, and derived object file list
SRCS = ${SRCDIR1}/matrix_op.c ${SRCDIR1}/priv.c ${SRCDIR1}/pub.c ${SRCDIR1}/gen.c ${SRCDIR1}/testInvert ${SRCDIR2}/client.c ${SRCDIR2}/server.c ${SRCDIR2}/testInvert.c ${SRCDIR1}/decrypt.c ${SRCDIR1}/encrypt.c ${SRCDIR1}/test_op.c${SRCDIR3}/testSmall.c ${SRCDIR3}/testLargeM.c ${SRCDIR3}/testLargeIS.c

OBJS = ${SRCDIR1}/test_op.o${SRCDIR1}/matrix_op.o ${SRCDIR1}/priv.o ${SRCDIR1}/pub.o ${SRCDIR1}/gen.o ${SRCDIR1}/testInvert.o ${SRCDIR2}/client.o ${SRCDIR2}/server.o ${SRCDIR2}/testInvert.o ${SRCDIR1}/decrypt.o ${SRCDIR1}/encrypt.o ${SRCDIR3}/testSmall.o ${SRCDIR3}/testLargeM.o ${SRCDIR3}/testLargeIS.o

${EXEC}:
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ggh.c ${LIBS}

compall: ggh.o ${OBJS} 
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ggh.o ${OBJS} ${LIBS}

.c.o:
	${CC} ${CFLAGS} ${INCLUDES} $< -o $@

depend: 
	makedepend ${SRCS} ggh.c

clean:
	find . -name "*.o" | xargs rm -v; rm -v ggh

tar:
	tar cf code.tar  Makefile *

help:
	echo "\nTo build: make clean;make\nCreate a tar file: make tar\nTo clean: make clean"

#print returns a .ps file called allcode giving a the code of the makefile and all files included in this program.
print:
	more Makefile $(HDRS) $(SRCS) | enscript -E --highlight=c --header="GGH Source Code" --color --font=Courier9 -jGp  sourcecode.ps 


#DO NOT DELETE
