CC:=g++
iglfw:=-I"C:\Program Files\Common Files\MinGW\glfw\include"
igl3w:=-I"C:\Program Files\Common Files\MinGW\gl3w\include"
iflags:=$(iglfw) $(igl3w)
lflags:=-L.
#libs:=-lopengl32 -lgdi32 -luser32 -lkernel32 -lglfw3# -Wl,--subsystem,windows
libs:=gl3w.o -lopengl32 -lglfw3# -Wl,--subsystem,windows
wflags:=-Wall -Wextra -pedantic -std=c++11
dflags:=-ggdb3
cflags:=$(iflags) $(wflags) -c -m64

name:=main

default:$(name).exe

$(name).exe:$(name).o shader.o
	$(CC) $(name).o shader.o -o $(name).exe $(lflags) $(libs)

$(name).o:$(name).cc
	$(CC) $(cflags) $(name).cc

shader.o:shader.cc
	$(CC) $(cflags) shader.cc
  
clean:
	rm $(name).o shader.o core $(name).exe