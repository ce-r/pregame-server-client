
#ifndef TTU_ChatBase_HPP
#define TTU_ChatBase_HPP

#include "TTU.hpp"

#include <openssl/ssl.h>
#include <netinet/in.h>
#include <string>
#include <set>


namespace gnetwork {
    class TTU_ChatBase : public TTU {
        protected:
            SSL_CTX* tls_context;
            SSL_CTX* dtls_context;
            bool is_server;
            
        public:
            TTU_ChatBase(const std::string &filen_crt, const std::string &filen_key, bool server);
            virtual ~TTU_ChatBase();
            void handle_tls_chat(SSL* ssl, bool is_server);
            void handle_dtls_chat(SSL_CTX* dtls_ctx, int udp_sock, struct sockaddr_in &peer_addr, bool is_server);
            SSL_CTX* get_tls_context() const;
            SSL_CTX* get_dtls_context() const;
    };
}

#endif

// 10