
#include "TestClient.hpp"
#include <string.h>


gnetwork::TestClient::TestClient() : BasicClient(AF_INET, SOCK_STREAM, 0, 80, inet_addr("127.0.0.1")) {}

void gnetwork::TestClient::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestClient::writer() {
    const char* hello = "Hello from client";
    if (write(new_socket, hello, strlen(hello)) < 0) {
        throw std::runtime_error("Failed to write to socket");
    }
    close(new_socket);
}

void gnetwork::TestClient::claunch() {
    std::cout << "Connecting..." << std::endl;
    new_socket = get_cli_socket()->conn_to_netw(get_cli_socket()->get_sock(), get_cli_socket()->get_address());
    
    if (new_socket < 0) {
        std::cerr << "Connection failed. Retrying...\n";
    }

    std::cout << "Connected!" << std::endl;

    // read server response
    read(new_socket, buffer, sizeof(buffer));
    print_buffer();
    writer();

    close(new_socket);
    std::cout << "Done." << std::endl;
}

void gnetwork::TestClient::launch() {
    std::cout << "Launching TestClient...\n";
    claunch();
}

