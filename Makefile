CPPFLAGS = -Wall -Wextra
PROGRAM = program

.PHONY: build clean debug program test leak_check docs release

default: build

build: clean $(PROGRAM)

clean:
	rm -rf *.o *.out *.exe docs $(PROGRAM) *.tar.gz

$(PROGRAM): main.o
	g++ $(CPPFLAGS) main.o -o $(PROGRAM)

main.o: main.cpp cbuffer.h
	g++ $(CPPFLAGS) -c main.cpp -o main.o

debug: CPPFLAGS += -g
debug: $(PROGRAM)

docs: 
	doxygen Doxyfile

test: debug
	./$(PROGRAM)

leak_check: debug
	valgrind --leak-check=yes ./$(PROGRAM)

# Only for my environment to build out the correct .tar.gz ready to be deployed
release: clean
	pandoc -f markdown Relazione.md -t latex -o Relazione.pdf
	rm -rf 807894
	mkdir 807894
	rsync -av --progress . 807894 --exclude=807894 --exclude=Qt --exclude=.git --exclude=.vscode --exclude=.gitignore --exclude=Esame-180219.pdf --exclude=*.tar.gz --exclude=Relazione.md
	rsync -av --progress Qt/persone/ 807894/Qt --exclude=persone.pro.user
	tar -cvzf 807894.tar.gz 807894