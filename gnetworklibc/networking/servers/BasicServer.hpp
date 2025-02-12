
#ifndef BasicServer_HPP
#define BasicServer_HPP

#include <stdio.h>
#include <unistd.h>

#include "../sockets/ListeningSocket.hpp"
#include "../gnetworklibc-networking.hpp"


namespace gnetwork {
    class BasicServer {
        private:
            ListeningSocket* serv_sock;
            
            virtual void acceptance() = 0;
            virtual void writer() = 0;
            virtual void print_buffer() = 0;

        public:
            BasicServer(int domain, int service, int protocol, int port, uint32_t interface, int bklg);
            virtual ~BasicServer();
            
            virtual void slaunch() = 0;  
            virtual void launch() = 0;
            
            ListeningSocket* get_serv_socket() const;
    };
}

#endif