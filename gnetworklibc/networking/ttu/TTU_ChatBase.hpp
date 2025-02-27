
#ifndef TTU_ChatBase_HPP
#define TTU_ChatBase_HPP

#include "TTU.hpp"

#include <openssl/ssl.h>
#include <string>

namespace gnetwork {
    class TTU_ChatBase : public TTU {
        protected:
            SSL_CTX* tls_context;
            bool is_server;
            
        public:
            TTU_ChatBase(const std::string &filen_crt, const std::string &filen_key, bool server);
            virtual ~TTU_ChatBase();
            void handle_tls_chat(SSL* ssl, bool is_server);
            SSL_CTX* get_tls_context() const;
    };
}

#endif

// 10