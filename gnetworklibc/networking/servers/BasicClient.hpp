
#ifndef BasicClient_HPP
#define BasicClient_HPP

#include <stdio.h>
#include <unistd.h>

#include "../sockets/ConnectingSocket.hpp"
#include "../gnetworklibc-networking.hpp"


namespace gnetwork {
    class BasicClient {
    private:
        ConnectingSocket* cli_socket;

    protected:  // make these methods accessible to derived classes
        virtual void writer() = 0;
        virtual void print_buffer() = 0;

    public:
        BasicClient(int domain, int service, int protocol, int port, uint32_t interface);
        virtual ~BasicClient();

        virtual void claunch() = 0;  
        virtual void launch() = 0;   

        ConnectingSocket* get_cli_socket() const;
    };
}

#endif