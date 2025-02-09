
#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream> 


namespace gnetwork {
    class Socket {
        private:
            int sock;
            struct sockaddr_in address;
        public: 
            Socket(int domain, int service, int protocol, int port, ulong interface);
            
            virtual int conn_to_netw(int sock, struct sockaddr_in) = 0; 
            
            void test_conn(int);
            
            int get_sock();
            struct sockaddr_in get_address();
    };
}

#endif