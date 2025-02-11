
#include "ConnectingSocket.hpp"


gnetwork::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, uint32_t interface) 
                                            : Socket(domain, service, protocol, port, interface) {};

gnetwork::ConnectingSocket::~ConnectingSocket() { 
    close(get_sock()); 
}

// int gnetwork::ConnectingSocket::conn_to_netw(int sock, const struct sockaddr_in& address) {
//     return connect(sock, (struct sockaddr*) &address, sizeof(address));
// }

int gnetwork::ConnectingSocket::conn_to_netw(int sock, const struct sockaddr_in& address) {
    const int MAX_ATTEMPTS = 5;
    int delay = 1;  // Initial delay in seconds

    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        // Set SO_REUSEADDR for quick socket reuse
        int optval = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
            perror("setsockopt(SO_REUSEADDR) failed");
            exit(EXIT_FAILURE);
        }

        // Disable Nagle's algorithm for low-latency data transfer
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) == -1) {
            perror("setsockopt(TCP_NODELAY) failed");
            exit(EXIT_FAILURE);
        }

        if (connect(sock, (struct sockaddr*)&address, sizeof(address)) == 0) {
            std::cout << "Successfully connected!" << std::endl;
            return sock;
        } else {
            // Print the IP and port for debugging
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(address.sin_addr), ip_str, INET_ADDRSTRLEN);
            fprintf(stderr, "Failed to connect to IP: %s, Port: %d (Attempt %d)\n", 
                    ip_str, ntohs(address.sin_port), attempt + 1);

            // Exponential backoff before retrying
            sleep(delay);
            delay *= 2;  // Exponential backoff (1s -> 2s -> 4s -> ...)
        }
    }

    std::cerr << "Failed to connect after multiple attempts. Exiting..." << std::endl;
    return -1;
}
