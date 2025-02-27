
#include "TTU_ChatBase.hpp"
#include <iostream>
#include <cstring>
#include <openssl/err.h>


gnetwork::TTU_ChatBase::TTU_ChatBase(const std::string &filen_crt, const std::string &filen_key, bool server) : 
                                     TTU(filen_crt, filen_key, server), is_server(server) {
    tls_context = create_context(is_server);

    if (is_server){
        configure_context(tls_context);
    }
}

gnetwork::TTU_ChatBase::~TTU_ChatBase() {
    SSL_CTX_free(tls_context);
}

SSL_CTX* gnetwork::TTU_ChatBase::get_tls_context() const {
    return tls_context;
}

void gnetwork::TTU_ChatBase::handle_tls_chat(SSL* ssl, bool is_server) {
    char buffer[1024];

    std::cout << (is_server ? "Server: Chat session started." : "Client: Chat session started.") << std::endl;

    while (true) {
        if (is_server) { 
            memset(buffer, 0, sizeof(buffer));
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

            if (msg == "/quit") 
                break;
            
            SSL_write(ssl, msg.c_str(), msg.length());

            memset(buffer, 0, sizeof(buffer));
            int bytes = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes <= 0) {
                std::cout << "Server disconnected or error occurred." << std::endl;
                break;
            }
            std::cout << "Server: " << buffer << std::endl;
        }
    }
}

// 55