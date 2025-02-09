
#ifndef ListeningSocket_hpp
#define ListeningSocket_hpp

#include <stdio.h>
#include "BindingSocket.hpp"


namespace gnetwork {
    class ListeningSocket : public BindingSocket {
        private:
            int backlog;
            int listening;
        public: 
            ListeningSocket(int domain, int service, int protocol, int port, ulong interface, int bklg);
            void _listening();
            int get_listening();
            int get_backlog();
    };
}

#endif