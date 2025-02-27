
#include <stdio.h>
#include "TestTCPServer.hpp"


int main() {
    gnetwork::TestTCPServer server;
    server.launch(); 
    return 0;
}


/*

fds:
0: stdin
1: stdout
2: stderr
3: and more may be opened in the program 
first fd opened takes value 3, second 4, third 5, etc...

*/ 