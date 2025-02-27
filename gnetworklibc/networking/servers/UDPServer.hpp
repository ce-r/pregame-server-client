
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "../sockets/BindingSocket.hpp"
#include <thread>

namespace gnetwork {
    class UDPServer {
        private:
            BindingSocket* udp_socket;
            std::thread udp_thread;
            bool running;

        public:
            UDPServer(int domain, int service, int protocol, int port, uint32_t interface);
            ~UDPServer();
            void udp_listener();
            void stop_udp_listener();
            BindingSocket* get_udp_socket() const;
    };
}

#endif
