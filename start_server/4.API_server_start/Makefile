COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
INCLUDE :=-Iusr/include/jack
LIBS :=-Lusr/lib/aarch64-linux-gnu -ljack -ljacknet -ljackserver

run: server_start.cpp
	$(COMPILER) $(OPTIONS) server_start.cpp $(INCLUDE) $(LIBS) -o run

clean:
	rm -f run
