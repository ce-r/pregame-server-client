
#ifndef AcceptingSocket_hpp
#define AcceptingSocket_hpp

#include "ListeningSocket.hpp"


namespace gnetwork {
    class AcceptingSocket : public ListeningSocket {
        private:
            int accept_sock;
                    
        public: 
            AcceptingSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg);
            ~AcceptingSocket();

            int accnetw(int sock, const struct sockaddr_in &server_address);
            int get_accepting() const;
    };
}

#endif