
#include "TU_Chat.hpp"

#define BUFFER_SIZE 1024


gnetwork::TU_Chat::TU_Chat() : udp_sock(-1), is_running(false) {
    memset(&udp_addr, 0, sizeof(udp_addr));
};

gnetwork::TU_Chat::~TU_Chat() {
    stop_udp_listener();
    if (udp_sock != -1) 
        close(udp_sock);
};

void gnetwork::TU_Chat::set_udp(int udp_socket, const sockaddr_in &addr) {
    udp_sock = udp_socket;
    udp_addr = addr;
    is_running = true;

    udp_listener_thread = std::thread(&TU_Chat::udp_listener, this);
}

void gnetwork::TU_Chat::udp_listener() {
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(udp_addr);

    while (is_running) {
        int bytes = recvfrom(udp_sock, buffer, BUFFER_SIZE, MSG_DONTWAIT,
                             (struct sockaddr*) &udp_addr, &addr_len);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "[UDP Received]: " << buffer << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // To prevent busy-waiting
    }
}

void gnetwork::TU_Chat::stop_udp_listener() {
    if (is_running) {
        is_running = false;
        if (udp_listener_thread.joinable()) {
            udp_listener_thread.join();
        }
    }
}

void gnetwork::TU_Chat::handle_tls_chat(SSL* ssl, bool is_server) {
    char buffer[BUFFER_SIZE]; // server
    while (true) {
        if (is_server) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes <= 0) {
                std::cout << "Client disconnected or error occurred." << std::endl;
                break;
            }

            std::cout << "Client: " << buffer << std::endl;

            std::string reply;
            std::cout << "Server: ";
            std::getline(std::cin, reply);
            SSL_write(ssl, reply.c_str(), reply.length());
        } else {
            std::string msg;
            std::cout << "Client: ";
            std::getline(std::cin, msg);

            if (msg == "/quit") {
                break;
            } else if (msg == "/stream") {
                const char* udp_msg = "Triggered UDP Stream Message!";
                sendto(udp_sock, udp_msg, strlen(udp_msg), 0,
                       (struct sockaddr*) &udp_addr, sizeof(udp_addr));
                std::cout << "[UDP Sent]: " << udp_msg << std::endl;
            } else {
                SSL_write(ssl, msg.c_str(), msg.length());

                memset(buffer, 0, BUFFER_SIZE);
                int bytes = SSL_read(ssl, buffer, sizeof(buffer));
                if (bytes <= 0) {
                    std::cout << "Server disconnected or error occurred." << std::endl;
                    break;
                }
                std::cout << "Server: " << buffer << std::endl;
            }
        }
    }
}
