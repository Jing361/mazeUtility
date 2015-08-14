CC:=g++
flags:=-Wall -Wextra -pedantic -std=c++14 -O2
dflags:=-ggdb3
cflags:=-c -m64

name:=main

default:$(name).exe

$(name).exe:$(name).o world.o AI.o
	$(CC) $(name).o world.o AI.o -o $(name).exe

$(name).o:$(name).cc
	$(CC) $(cflags) $(flags) $(name).cc
  
world.o:world.cc
	$(CC) $(cflags) $(flags) world.cc
  
AI.o:AI.cc
	$(CC) $(cflags) $(flags) AI.cc

clean:
	rm *.o core $(name).exe
