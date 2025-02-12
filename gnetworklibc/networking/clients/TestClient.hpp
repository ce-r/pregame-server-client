
#ifndef TestClient_HPP
#define TestClient_HPP

#include <stdio.h>
#include "BasicClient.hpp"


namespace gnetwork {
    class TestClient : public BasicClient {
        private:
            char buffer[30000] = {0};
            int new_socket;
            void writer() override;
            void print_buffer() override;
        
        public:
            TestClient();
            void claunch() override;
            void launch() override;
    };  
}

#endif