
#ifndef TestTCPClient_HPP
#define TestTCPClient_HPP

#include <stdio.h>
#include "TCPClient.hpp"


namespace gnetwork {
    class TestTCPClient : public TCPClient {
        private:
            char buffer[30000] = {0};
            int new_socket;
            void reader() override;
            void writer() override;
            void print_buffer() override;
        
        public:
            TestTCPClient();
            void claunch() override;
            void launch() override;
    };  
}

#endif