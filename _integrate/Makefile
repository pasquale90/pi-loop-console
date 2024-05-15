COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra #-Werror
INCLUDE :=-Iusr/include #-I/usr/include/x86_64-linux-gnu
LIBS :=-L/usr/lib -ljack -ljacknet -ljackserver
OBJECTS := audioserver.o main.o

run_server: $(OBJECTS)
	$(COMPILER) $(OPTIONS) $(OBJECTS) $(LIBS) -o run_server

main.o: main.cpp
	$(COMPILER) -c $(OPTIONS) main.cpp $(INCLUDE) -o main.o $(LIBS)

audioserver.o: audioserver.h audioserver.cpp
	$(COMPILER) -c $(OPTIONS) audioserver.cpp $(INCLUDE) -o audioserver.o $(LIBS)

clean:
	rm -f run_server ./*.o

