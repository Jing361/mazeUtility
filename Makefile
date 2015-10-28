CC:=g++
inclloc:=include
libloc:=lib
objloc:=obj

iglfw:=-I$(inclloc)/glfw/include
igl3w:=-I$(inclloc)/gl3w/include
iglm:=-I$(inclloc)/glm
iself:=-I./$(inclloc)
iflags:=$(iglfw) $(igl3w) $(iglm) $(iself)
lflags:=-L./$(libloc) -L.
#libs:=-lopengl32 -lgdi32 -luser32 -lkernel32 -lglfw3# -Wl,--subsystem,windows
libs:=$(libloc)/gl3w.o -lSOIL -lopengl32 -lglfw3# -Wl,--subsystem,windows
wflags:=-Wall -Wextra -pedantic -std=c++14 -O2
dflags:=-ggdb3
cflags:=$(iflags) $(wflags) -c -m64

name:=main
mazeTest:=maze
mazeGame:=game

default:$(name).exe

mazetest:$(mazeTest).exe

mazegame:$(mazeGame).exe

test:struct.exe

all:default mazetest mazegame

struct.exe:$(objLoc)/struct.o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/entity.o $(objloc)/resourceManager.o $(objloc)/sceneManager.o $(objloc)/sceneNode.o
	$(CC) $(objLoc)/struct.o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/entity.o $(objloc)/resourceManager.o $(objloc)/sceneManager.o $(objloc)/sceneNode.o -o struct.exe $(lflags) $(libs)

$(mazeGame).exe:$(objloc)/$(mazeGame).o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/AI.o $(objloc)/world.o $(objloc)/stuff.o $(objloc)/model.o
	$(CC) $(objloc)/$(mazeGame).o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/AI.o $(objloc)/world.o $(objloc)/stuff.o $(objloc)/model.o -o $(mazeGame).exe $(lflags) $(libs)

$(name).exe:$(objloc)/$(name).o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/spotLight.o $(objloc)/fileLoader.o $(objloc)/glGame.o $(objloc)/model.o
	$(CC) $(objloc)/$(name).o $(objloc)/shader.o $(objloc)/camera.o $(objloc)/light.o $(objloc)/spotLight.o $(objloc)/fileLoader.o $(objloc)/glGame.o $(objloc)/model.o -o $(name).exe $(lflags) $(libs)

$(objLoc)/struct.o:struct.cc
	$(CC) $(cflags) struct.cc -o $(objloc)/struct.o

$(objloc)/entity.o:entity.cc
	$(CC) $(cflags) entity.cc -o $(objloc)/entity.o

$(objloc)/resourceManager.o:resourceManager.cc
	$(CC) $(cflags) resourceManager.cc -o $(objloc)/resourceManager.o

$(objloc)/sceneManager.o:sceneManager.cc
	$(CC) $(cflags) sceneManager.cc -o $(objloc)/sceneManager.o

$(objloc)/sceneNode.o:sceneNode.cc
	$(CC) $(cflags) sceneNode.cc -o $(objloc)/sceneNode.o

$(objloc)/model.o:model.cc
	$(CC) $(cflags) model.cc -o $(objloc)/model.o

$(objloc)/glGame.o:glGame.cc
	$(CC) $(cflags) glGame.cc -o $(objloc)/glGame.o

$(objloc)/fileLoader.o:fileLoader.cc
	$(CC) $(cflags) fileLoader.cc -o $(objloc)/fileLoader.o
  
$(objloc)/$(name).o:$(name).cc model.cc
	$(CC) $(cflags) $(name).cc -o $(objloc)/$(name).o

$(objloc)/$(mazeGame).o:$(mazeGame).cc model.cc
	$(CC) $(cflags) $(mazeGame).cc -o $(objloc)/$(mazeGame).o

$(objloc)/shader.o:shader.cc
	$(CC) $(cflags) shader.cc -o $(objloc)/shader.o
  
$(objloc)/camera.o:camera.cc
	$(CC) $(cflags) camera.cc -o $(objloc)/camera.o

$(objloc)/light.o:light.cc
	$(CC) $(cflags) light.cc -o $(objloc)/light.o

$(objloc)/spotLight.o:spotLight.cc
	$(CC) $(cflags) spotLight.cc -o $(objloc)/spotLight.o

$(mazeTest).exe:$(objloc)/$(mazeTest).o $(objloc)/world.o $(objloc)/AI.o $(objloc)/stuff.o
	$(CC) $(objloc)/$(mazeTest).o $(objloc)/world.o $(objloc)/AI.o $(objloc)/stuff.o -o $(mazeTest).exe

$(objloc)/$(mazeTest).o:$(mazeTest).cc
	$(CC) $(cflags) $(flags) $(mazeTest).cc -o $(objloc)/$(mazeTest).o
  
$(objloc)/world.o:world.cc
	$(CC) $(cflags) $(flags) world.cc -o $(objloc)/world.o
  
$(objloc)/AI.o:AI.cc
	$(CC) $(cflags) $(flags) AI.cc -o $(objloc)/AI.o

$(objloc)/stuff.o:stuff.cc
	$(CC) $(cflags) $(flags) stuff.cc -o $(objloc)/stuff.o

clean:
	rm $(objloc)/* core $(name).exe $(mazeTest).exe $(mazeGame).exe
