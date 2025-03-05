
#ifndef AcceptingSocket_HPP
#define AcceptingSocket_HPP

#include "ListeningSocket.hpp"


namespace gnetwork {
    class AcceptingSocket : public ListeningSocket {
        private:
            int accept_sock;
                    
        public: 
            AcceptingSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg);
            ~AcceptingSocket();

            int acceptserv(int sock, const struct sockaddr_in &server_address);
            int get_accepting() const;
    };
}

#endif

// 10