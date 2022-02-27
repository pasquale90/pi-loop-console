#include <jack/jack.h>
#include <jack/types.h>
#include <jack/control.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

int main ()
{
    std::cout<<"Hello Piloop"<<std::endl;
     jackctl_server_t * server;
//
std::ofstream cout_log ("logs/cout.log");
std::ofstream cerr_log ("logs/cerr.log");

// connect stream buffers
std::streambuf *coutbuf = std::cout.rdbuf();
std::cout.rdbuf(cout_log.rdbuf () );
std::streambuf *cerrbuf = std::cerr.rdbuf();
std::cerr.rdbuf(cerr_log.rdbuf () );


    



// restore
std::cout.flush ();
std::cout.rdbuf(cerrbuf);

std::cerr.flush ();
std::cerr.rdbuf(cerrbuf);

        return 0;
}
