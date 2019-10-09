FLAGS=-Wall -g
CC=gcc
CFLAGS=$(FLAGS) -I. -O2 -I./lista/ -I./arbol/

DEPS = lista/lista.h arbol/arbol.h
LIBRERIA = liblibreria.so
PATHLIBRERIA = $(shell pwd)

OBJECTSARBOL = arbol/arbol.o
OBJECTSLISTA = lista/lista.o
OBJECTSTEST = lista/lista_test.o arbol/arbol_test.o
OBJECTS = $(OBJECTSLISTA) $(OBJECTSARBOL) $(OBJECTSTEST)

lista/%.o: lista/%.c $(DEPS)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

arbol/%.o: arbol/%.c $(DEPS)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

libreria:
	ld -g -o $(LIBRERIA) $(OBJECTSARBOL) $(OBJECTSLISTA) $(OBJECTSTEST) -shared

build: $(OBJECTS) libreria
	$(CC) $(CFLAGS) -o main.o -c main.c
	$(CC) $(FLAGS) -o tateti main.o -L$(PATHLIBRERIA) -llibreria

install:
	sudo cp $(PATHLIBRERIA) /usr/lib/
	sudo ldconfig

run: build
	LD_LIBRARY_PATH=$(shell pwd) ./tateti

clean:
	rm $(OBJECTS) main.o