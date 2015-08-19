CC:=g++
iglfw:=-I"C:\Program Files\Common Files\MinGW\glfw\include"
igl3w:=-I"C:\Program Files\Common Files\MinGW\gl3w\include"
iglm:=-I"C:\Program Files\Common Files\MinGW\glm"
iflags:=$(iglfw) $(igl3w) $(iglm)
lflags:=-L.
#libs:=-lopengl32 -lgdi32 -luser32 -lkernel32 -lglfw3# -Wl,--subsystem,windows
libs:=gl3w.o -lSOIL -lopengl32 -lglfw3# -Wl,--subsystem,windows
wflags:=-Wall -Wextra -pedantic -std=c++14 -O2
dflags:=-ggdb3
cflags:=$(iflags) $(wflags) -c -m64

name:=main

default:$(name).exe

$(name).exe:$(name).o shader.o camera.o light.o
	$(CC) $(name).o shader.o camera.o light.o -o $(name).exe $(lflags) $(libs)

$(name).o:$(name).cc model.cc
	$(CC) $(cflags) $(name).cc

shader.o:shader.cc
	$(CC) $(cflags) shader.cc
  
camera.o:camera.cc
	$(CC) $(cflags) camera.cc

light.o:light.cc
	$(CC) $(cflags) light.cc

clean:
	rm $(name).o shader.o model.o camera.o light.o core $(name).exe
