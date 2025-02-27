
#ifndef TestTCPServer_HPP
#define TestTCPServer_HPP

#include <stdio.h>
#include "TCPServer.hpp"


namespace gnetwork {
    class TestTCPServer : public TCPServer {
        private:
            char buffer[30000] = {0};
            int new_socket;
            void reader();
            void writer();
            void print_buffer();
        
        public:
            TestTCPServer();
            void slaunch() override; 
            void launch() override;
    };  
}

#endif

