
#ifndef TestClient_HPP
#define TestClient_HPP

#include <stdio.h>
#include "BasicClient.hpp"


namespace gnetwork {
    class TestClient : public BasicClient {
        private:
            // ConnectingSocket* socket;
            char buffer[30000] = {0};
            int new_socket;
            // void connection();
            // void writer();
            // void print_buffer();
            void writer() override;
            void print_buffer() override;
        
        public:
            TestClient();
            // void claunch();
            // void launch();
            void claunch() override;
            void launch() override;
    };  
}

#endif