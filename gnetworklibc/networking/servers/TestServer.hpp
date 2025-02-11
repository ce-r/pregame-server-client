
#ifndef TestServer_HPP
#define TestServer_HPP

#include <stdio.h>
#include "BasicServer.hpp"


namespace gnetwork {
    class TestServer : public BasicServer {
        private:
            char buffer[30000] = {0};
            int new_socket;
            void acceptance();
            void writer();
            void print_buffer();
        
        public:
            TestServer();
            void slaunch();
    };  
}

#endif