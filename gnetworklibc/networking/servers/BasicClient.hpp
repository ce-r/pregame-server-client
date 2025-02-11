
#ifndef BasicClient_HPP
#define BasicClient_HPP

#include <stdio.h>
#include <unistd.h>

#include "../sockets/ConnectingSocket.hpp"
#include "../gnetworklibc-networking.hpp"


// namespace gnetwork {
//     class BasicClient {
//         private:
//             // ConnectingSocket* socket;
//             // virtual void connection() = 0;
//             ConnectingSocket* cli_socket;
//             virtual void writer() = 0;
//             virtual void print_buffer() = 0;
//         public:
//             BasicClient(int domain, int service, int protocol, int port, uint32_t interface);
//             virtual ~BasicClient();
//             virtual void claunch() = 0;
//             // ConnectingSocket* get_socket() const;
//             ConnectingSocket* get_cli_socket() const;
//     };
// }

// #endif



namespace gnetwork {
    class BasicClient {
    private:
        ConnectingSocket* cli_socket;

    protected:  // Make these accessible to derived classes
        virtual void writer() = 0;
        virtual void print_buffer() = 0;

    public:
        BasicClient(int domain, int service, int protocol, int port, uint32_t interface);
        virtual ~BasicClient();

        virtual void claunch() = 0;  // Pure virtual
        virtual void launch() = 0;   // Also pure virtual for proper overriding

        ConnectingSocket* get_cli_socket() const;
    };
}

#endif