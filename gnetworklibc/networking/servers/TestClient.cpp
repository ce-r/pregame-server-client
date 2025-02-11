
#include "TestClient.hpp"
#include <string.h>


gnetwork::TestClient::TestClient() : BasicClient(AF_INET, SOCK_STREAM, 0, 80, inet_addr("127.0.0.1")) {
    // claunch();
}

// void gnetwork::TestClient::acceptance() {
//     struct sockaddr_in address = get_socket()->get_address();
//     int addrlen = sizeof(address);
//     new_socket = accept(get_socket()->get_sock(), (struct sockaddr*) &address, (socklen_t*) &addrlen);
//     if (new_socket < 0) {
//         throw std::runtime_error("Failed to accept connection");
//     }
//     read(new_socket, buffer, 30000);
// }

void gnetwork::TestClient::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestClient::writer() {
    const char* hello = "hello from client";
    // write(new_socket, hello, strlen(hello));
    if (write(new_socket, hello, strlen(hello)) < 0) {
        throw std::runtime_error("Failed to write to socket");
    }
    close(new_socket);
}

// void gnetwork::TestClient::claunch() {
//     while (true) {
//         std::cout << "==== connecting ====" << std::endl;
//         // acceptance();
//         new_socket = get_cli_socket()->conn_to_netw(get_cli_socket()->get_sock(), get_cli_socket()->get_address());
        
//         if (new_socket < 0) {
//             std::cerr << "Connection failed. Retrying...\n";
//             continue;
//         }

//         std::cout << "==== connected ====" << std::endl;

//         // Read server response
//         read(new_socket, buffer, sizeof(buffer));
//         print_buffer();
//         writer();

//         std::cout << "==== done ====" << std::endl;
//     }
// }

void gnetwork::TestClient::claunch() {
    std::cout << "==== connecting ====" << std::endl;
    // acceptance();
    new_socket = get_cli_socket()->conn_to_netw(get_cli_socket()->get_sock(), get_cli_socket()->get_address());
    
    if (new_socket < 0) {
        std::cerr << "Connection failed. Retrying...\n";
    }

    std::cout << "==== connected ====" << std::endl;

    // Read server response
    read(new_socket, buffer, sizeof(buffer));
    print_buffer();
    writer();

    close(new_socket);
    std::cout << "==== done ====" << std::endl;
}

void gnetwork::TestClient::launch() {
    std::cout << "Launching TestClient...\n";
    claunch();
}

