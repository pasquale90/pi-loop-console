COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
COMPILE :=$(COMPILER) $(NOPTIONS)


#server_control:
#	$(COMPILE) server_control.o -Iusr/include -ljack -o server_control

server_control:server_control.cpp jack/jack.h jack/control.h
	$(COMPILER) $(NOPTIONS) server_control.cpp -I. -Lusr/lib/aarch64-linux-gnu -ljack -ljackserver -ljacknet -o server_control

clean:
	rm -f server_control.o server_control
















