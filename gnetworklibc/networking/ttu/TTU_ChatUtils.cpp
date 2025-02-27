
#include "TTU_ChatUtils.hpp"


void gnetwork::TTU_ChatUtils::log_message(const std::string &message, bool is_server) {
    std::cout << (is_server ? "Server: " : "Client: ") << message << std::endl;
}

std::string gnetwork::TTU_ChatUtils::get_user_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// 5
// total 160

// running total ~620