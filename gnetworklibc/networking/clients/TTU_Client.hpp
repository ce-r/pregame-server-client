// previously BasicClient.hpp
// HPP specific to TCP-TLS-UDP chat

#ifndef TTU_Client_HPP
#define TTU_Client_HPP

#include "../ttu/TTU_ChatBase.hpp"
#include "../clients/TCPClient.hpp"
#include "../clients/UDPClient.hpp"


namespace gnetwork {
    class TTU_Client : public TTU_ChatBase, public TCPClient, public UDPClient {
        public:
            TTU_Client(int domain, int service, int protocol, int port, uint32_t interface);
            void launch();
    };
}

#endif

// 5