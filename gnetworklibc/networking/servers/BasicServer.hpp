#ifndef BasicServer_hpp
#define BasicServer_hpp

#include <stdio.h>
#include <unistd.h>

#include "../sockets/ListeningSocket.hpp"
#include "../gnetworklibc-networking.hpp"


namespace gnetwork {
    class BasicServer {
        private:
            ListeningSocket* socket;
            virtual void acceptance() = 0;
            virtual void print_buffer() = 0;
            virtual void writer() = 0;
        public:
            BasicServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
            virtual void launch() = 0;
            ListeningSocket* get_socket();
    };
}

#endif