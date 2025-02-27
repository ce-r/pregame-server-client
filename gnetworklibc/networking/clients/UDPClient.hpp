
#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "../sockets/ConnectingSocket.hpp"

namespace gnetwork {
    class UDPClient {
        private:
            ConnectingSocket* udp_socket;
            struct sockaddr_in server_address;

        public:
            UDPClient(int domain, int service, int protocol, int port, uint32_t interface);
            ~UDPClient();
            void send_udp_message(const std::string &msg);
    };
}

#endif
