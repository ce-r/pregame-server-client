// previously BasicServer.hpp
// HPP specific to TCP-TLS-UDP chat

#ifndef TTU_Server_HPP
#define TTU_Server_HPP

#include "../ttu/TTU_ChatBase.hpp"
#include "TCPServer.hpp"
#include "UDPServer.hpp"
#include <thread>
#include <vector>

namespace gnetwork {
    class TTU_Server : public TTU_ChatBase, public UDPServer, public TCPServer {
        public:
            TTU_Server(const std::string &filen_crt, const std::string &filen_key, int domain, 
                      int service, int protocol, int port, uint32_t interface, int bklg);
            void launch();
            // void udp_listener();
    };
}

#endif

// 20