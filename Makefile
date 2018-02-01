CPPFLAGS = -Wall -Wextra
PROGRAM = program

.PHONY: build clean debug program test leak_check

default: debug

build: clean $(PROGRAM)

clean:
	rm -f *.o *.out *.exe

$(PROGRAM): main.o
	g++ $(CPPFLAGS) main.o -o $(PROGRAM)

main.o: main.cpp cbuffer.h
	g++ $(CPPFLAGS) -c main.cpp -o main.o

debug: CPPFLAGS += -g
debug: $(PROGRAM)

test: debug
	./$(PROGRAM)

leak_check: debug
	valgrind --leak-check=yes ./$(PROGRAM)