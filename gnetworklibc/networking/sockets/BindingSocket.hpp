
#ifndef BindingSocket_HPP
#define BindingSocket_HPP

#include "Socket.hpp"


namespace gnetwork {
    class BindingSocket : public Socket {
        private:
            int binding;
            int socket_type;
            int bind2self(int sock, const struct sockaddr_in &address) override;
            // int bind2sock(int sock, const struct sockaddr_in &address);
        
        public: 
            BindingSocket(int domain, int service, int protocol, int port, uint32_t interface);
            ~BindingSocket();
            int get_binding() const;
    };
}

#endif

// 10