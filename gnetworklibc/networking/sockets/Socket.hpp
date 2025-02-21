
#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream> 
#include <cstdint>


namespace gnetwork {
    class Socket {
        private:
            int sock;
            struct sockaddr_in address;

        public: 
            Socket(int domain, int service, int protocol, int port, uint32_t interface);
            virtual ~Socket();

            virtual int conn_to_netw(int sock, const struct sockaddr_in &address) = 0; 
            
            void test_conn(int sock);
            
            struct sockaddr_in get_address() const;
            int get_sock() const;
    };
}

#endif