
#ifndef ConnectingSocket_HPP
#define ConnectingSocket_HPP

#include <stdio.h>
#include "Socket.hpp"


namespace gnetwork {
    class ConnectingSocket : public Socket {
        public: 
            ConnectingSocket(int domain, int service, int protocol, int port, uint32_t interface);
            ~ConnectingSocket();
            int conn2serv(int sock, const struct sockaddr_in &address); 
            // Empty implementation to satisfy pure virtual requirement
            // int bind2netw(int sock, const struct sockaddr_in &address) override;
            int bind2self(int sock, const struct sockaddr_in &address) override;
    };
}

#endif

// 5