#ifndef TU_Chat_HPP
#define TU_Chat_HPP

// #include "ListeningSocket.hpp"
#include "TTU.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <atomic>


namespace gnetwork {
    class TU_Chat {
        private:
            int udp_sock; 
            struct sockaddr_in udp_addr;
            std::atomic<bool> is_running;
            std::thread udp_listener_thread;
            void udp_listener();

        public: 
            TU_Chat(); // cert and key
            ~TU_Chat();

            void handle_tls_chat(SSL* ssl, bool is_server);
            void set_udp(int udp_socket, const sockaddr_in& addr);
            void stop_udp_listener();
    };
}

#endif