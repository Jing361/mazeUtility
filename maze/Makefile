CC:=g++
flags:=-Wall -Wextra -pedantic -O2 -std=c++0x
dflags:=-ggdb3
cflags:=-c $(dflags)

name:=main

default:$(name).exe

$(name).exe:$(name).o
	$(CC) $(name).o -o $(name).exe

$(name).o:$(name).cc
	$(CC) $(cflags) $(flags) $(name).cc

clean:
	del *.o core $(name).exe
