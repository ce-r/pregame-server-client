
#include "ConnectingSocket.hpp"
#include <string.h>


gnetwork::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, uint32_t interface) 
                                            : Socket(domain, service, protocol, port, interface) {
    struct sockaddr_in address = get_address(); // must be modifiable
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = interface;
};

gnetwork::ConnectingSocket::~ConnectingSocket() { 
    close(get_sock()); 
}

int gnetwork::ConnectingSocket::conn2netw(int sock, const struct sockaddr_in &address) {
    const int MAX_ATTEMPTS = 5;
    int delay = 1;  // Initial delay in seconds

    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        if (connect(sock, (struct sockaddr*) &address, sizeof(address)) == 0) {
            std::cout << "Successfully connected!" << std::endl;
            return sock;
        } else {
            perror("Connect failed");
            sleep(delay);
            delay *= 2;  // Exponential backoff
        }
    }

    std::cerr << "Failed to connect after multiple attempts. Exiting..." << std::endl;
    return -1;
}

int gnetwork::ConnectingSocket::bind2netw(int sock, const struct sockaddr_in &address) {
    // Clients typically don't bind, so this can be empty or return success
    return 0;
}