
#ifndef TTU_ChatUtils_HPP
#define TTU_ChatUtils_HPP

#include <string>
#include <iostream>

namespace gnetwork {
    class TTU_ChatUtils {
        public:
            static void log_message(const std::string &message, bool is_server);
            static std::string get_user_input();
    };
}

#endif


// 5