
#ifndef TCPServer_HPP
#define TCPServer_HPP

#include <stdio.h>
#include <unistd.h>

// #include "../sockets/ListeningSocket.hpp"
#include "../sockets/AcceptingSocket.hpp"
#include "../gnetworklibc-networking.hpp"


namespace gnetwork {
    class TCPServer {
        private:
            // ListeningSocket* serv_sock;
            AcceptingSocket* serv_sock;
            
            // virtual void reader() = 0;
            // virtual void writer() = 0;
            // virtual void print_buffer() = 0;
            // virtual void reader();
            // virtual void writer();
            // virtual void print_buffer();

        public:
            TCPServer(int domain, int service, int protocol, int port, uint32_t interface, int bklg);
            virtual ~TCPServer();
            
            // virtual void slaunch() = 0; 
            // virtual void slaunch();  
            virtual void launch() = 0;
            
            // ListeningSocket* get_serv_socket() const;
            AcceptingSocket* get_serv_socket() const;
    };
}

#endif

// 10