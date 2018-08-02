## Compiler
CC=g++
LFLAGS = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm
assn3: main.o ObjLoader.o Transfer.o Vector3f.o Material.o Ray.o RGBColor.o Camera.o TSample.o
	${CC} main.o ObjLoader.o Transfer.o Vector3f.o Material.o Ray.o RGBColor.o Camera.o TSample.o -o assn3 ${LFLAGS}
main.o: main.cpp
	${CC} -c main.cpp ${LFLAGS}
Transfer.o: Transfer.cpp
	${CC} -c Transfer.cpp
ObjLoader.o: ObjLoader.cpp
	${CC} -c ObjLoader.cpp
Vector3f.o: Vector3f.cpp
	${CC} -c Vector3f.cpp
Material.o: Material.cpp
	${CC} -c Material.cpp
Ray.o: Ray.cpp
	${CC} -c Ray.cpp
RGBColor.o: RGBColor.cpp
	${CC} -c RGBColor.cpp
Camera.o: Camera.cpp
	${CC} -c Camera.cpp
TSample.o: TSample.cpp
	${CC} -c TSample.cpp	

clean:
	rm *.o
#DO NOT DELETE
