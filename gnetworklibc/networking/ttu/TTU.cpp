
#include "TTU.hpp"
#include <string>


gnetwork::TTU::TTU(const std::string &filen_crt, const std::string &filen_key, bool is_server) 
                : server_crt(filen_crt), server_key(filen_key), ctx(nullptr) {
    init_openssl();
    ctx = create_context(is_server);  // pass `is_server` instead of always assuming `true`
};

gnetwork::TTU::~TTU() {
    if (ctx) {
        SSL_CTX_free(ctx);
    }
    cleanup_openssl();
};

void gnetwork::TTU::init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void gnetwork::TTU::cleanup_openssl() {
    EVP_cleanup();
}

// DTLS instead of TLS
SSL_CTX* gnetwork::TTU::create_context(bool is_server) { // server and client 
    const SSL_METHOD* method = is_server ? TLS_server_method() : TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to create SSL context");
    }

    if (is_server) { 
        if (SSL_CTX_use_certificate_file(ctx, server_crt.c_str(), SSL_FILETYPE_PEM) <= 0 ||
            SSL_CTX_use_PrivateKey_file(ctx, server_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            throw std::runtime_error("Failed to configure SSL context");
        }
    }
    
    return ctx;
}

SSL_CTX* gnetwork::TTU::get_context() const { 
    return ctx; 
}

std::string gnetwork::TTU::get_crt() {
    return server_crt;
}

std::string gnetwork::TTU::get_key() {
    return server_key;
}

// 65