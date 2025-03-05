
#ifndef ListeningSocket_HPP
#define ListeningSocket_HPP

#include <stdio.h>
#include "BindingSocket.hpp"


namespace gnetwork {
    class ListeningSocket : public BindingSocket {
        private:
            int backlog;
            int listening;
        
        public: 
            ListeningSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg);
            ~ListeningSocket();
            void listen4client();
            int get_listening();
            int get_backlog();
    };
}

#endif

// 10