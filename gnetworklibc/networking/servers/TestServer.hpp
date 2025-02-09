
#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include "BasicServer.hpp"


namespace gnetwork {
    class TestServer : public BasicServer {
        private:
            char buffer[30000] = {0};
            int new_socket;
            void acceptance();
            void print_buffer();
            void writer();

        public:
            TestServer();
            void launch();
    };
        
}

#endif