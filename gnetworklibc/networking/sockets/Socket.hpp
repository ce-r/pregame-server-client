
#ifndef Socket_HPP
#define Socket_HPP

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
            int socket_type;

        public: 
            Socket(int domain, int service, int protocol, int port, uint32_t interface);
            virtual ~Socket();

            virtual int bind2netw(int sock, const struct sockaddr_in &address) = 0; 
            
            void test_sock(int sock);
            
            struct sockaddr_in get_address() const;
            void set_address(const struct sockaddr_in &addr);
            int get_sock() const;
            int get_socket_type() const; 
    };
}

#endif