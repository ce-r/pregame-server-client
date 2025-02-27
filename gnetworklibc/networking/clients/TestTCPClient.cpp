
#include "TestTCPClient.hpp"
#include <string.h>


gnetwork::TestTCPClient::TestTCPClient() : TCPClient(AF_INET, SOCK_STREAM, 0, 8080, inet_addr("127.0.0.1")) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    get_cli_socket()->set_address(addr);
}

void gnetwork::TestTCPClient::print_buffer() {
    std::cout << buffer << std::endl;
}

void gnetwork::TestTCPClient::writer() {
    if (new_socket <= 0) {
        throw std::runtime_error("Invalid socket. Cannot write.");
    }

    const char* hello = "Hello from client!";
    ssize_t bytes_written = write(new_socket, hello, strlen(hello));

    if (bytes_written < 0) {
        perror("Write failed");
        throw std::runtime_error("Failed to write to socket");
    } else {
        std::cout << "Message sent to server." << std::endl;
    }
}

void gnetwork::TestTCPClient::reader() {
    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Read failed or server closed connection");
        close(new_socket);
        throw std::runtime_error("Failed to read from socket");
    }

    buffer[bytes_read] = '\0';  // Null-terminate the buffer
    std::cout << "Received from server: " << buffer << std::endl;
}

void gnetwork::TestTCPClient::claunch() {
    std::cout << "Connecting..." << std::endl;
    new_socket = get_cli_socket()->conn2netw(get_cli_socket()->get_sock(), get_cli_socket()->get_address());

    if (new_socket < 0) {
        std::cerr << "Connection failed. Retrying...\n";
        return;
    }

    std::cout << "Connected!" << std::endl;

    // Send data to server
    writer();

    // Read server response before closing the socket
    try {
        reader();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    // Close the socket after read/write is complete
    if (new_socket > 0) {
        if (close(new_socket) < 0) {
            perror("Failed to close client socket");
        } else {
            std::cout << "Client socket closed successfully." << std::endl;
            new_socket = -1;  // Mark as closed
        }
    }

    std::cout << "Done." << std::endl;
}

void gnetwork::TestTCPClient::launch() {
    std::cout << "Launching TestClient...\n";
    claunch();
}



// the listening socket fd: 3 listens on port 8080
// when the client connects, the server creates a new socket fd: 4 for that connection.
// listening socket fd 3 continues to listen for other clients
// while new calls to accept require a new fd

// i.e.
// Listening socket FD: 3
// Client 1 connected -> new FD: 4
// Client 2 connected -> new FD: 5
// Client 3 connected -> new FD: 6