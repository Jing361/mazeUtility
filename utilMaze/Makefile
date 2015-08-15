CC:=g++
flags:=-Wall -Wextra -pedantic -std=c++11 -O2
dflags:=-ggdb3
cflags:=-c -m64

name:=main

default:$(name).exe

$(name).exe:$(name).o world.o AI.o stuff.o
	$(CC) $(name).o world.o AI.o stuff.o -o $(name).exe

$(name).o:$(name).cc
	$(CC) $(cflags) $(flags) $(name).cc
  
world.o:world.cc
	$(CC) $(cflags) $(flags) world.cc
  
AI.o:AI.cc
	$(CC) $(cflags) $(flags) AI.cc

stuff.o:stuff.cc
	$(CC) $(cflags) $(flags) stuff.cc
  
clean:
	rm *.o core $(name).exe
