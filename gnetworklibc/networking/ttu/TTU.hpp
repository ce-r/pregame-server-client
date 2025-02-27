
#ifndef TTU_HPP
#define TTU_HPP

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>

namespace gnetwork {
    class TTU {
        private:
            std::string server_crt; 
            std::string server_key;
            SSL_CTX* ctx;
            
        public: 
            TTU(const std::string &filen_crt, const std::string &filen_key, bool is_server); // cert and key
            ~TTU();

            void init_openssl();
            void cleanup_openssl();
            SSL_CTX* create_context(bool is_server);

            void configure_context(SSL_CTX* ctx);
            SSL_CTX* get_context() const;

            std::string get_crt(); 
            std::string get_key();
    };
}

#endif

// 20