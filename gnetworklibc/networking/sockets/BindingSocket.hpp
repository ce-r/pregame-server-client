
#ifndef BindingSocket_hpp
#define BindingSocket_hpp

#include <stdio.h>
#include "Socket.hpp"


namespace gnetwork {
    class BindingSocket : public Socket {
        private:
            int binding;
            int conn_to_netw(int sock, struct sockaddr_in address) override;
        public: 
            BindingSocket(int domain, int service, int protocol, int port, ulong interface);
            int get_binding();
    };
}

#endif