
#ifndef TCPClient_HPP
#define TCPClient_HPP

#include <stdio.h>
#include <unistd.h>

#include "../sockets/ConnectingSocket.hpp"
#include "../gnetworklibc-networking.hpp"


namespace gnetwork {
    class TCPClient {
        private:
            ConnectingSocket* cli_socket;

        protected:  // make these methods accessible to derived classes
            // virtual void reader();
            // virtual void writer();
            // virtual void print_buffer();

        public:
            TCPClient(int domain, int service, int protocol, int port, uint32_t interface);
            virtual ~TCPClient();

            // virtual void claunch();  
            virtual void launch() = 0;   

            ConnectingSocket* get_cli_socket() const;
    };
}

#endif