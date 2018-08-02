## Compiler
CC=g++
LFLAGS = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm
assn2: main.o ObjLoader.o Transfer.o ZBuffer.o
	${CC} main.o Transfer.o ObjLoader.o ZBuffer.o -o assn2 ${LFLAGS}
main.o: main.cpp
	${CC} -c main.cpp ${LFLAGS}
Transfer.o: Transfer.cpp
	${CC} -c Transfer.cpp
ObjLoader.o: ObjLoader.cpp
	${CC} -c ObjLoader.cpp
ZBuffer.o: ZBuffer.cpp
	${CC} -c ZBuffer.cpp


clean:
	rm *.o
#DO NOT DELETE
