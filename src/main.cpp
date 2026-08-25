#include "WafSocket.h"

int main(){
    std::cout << "Initializing WAF..." << std::endl;

    WafSocket test_socket;

    // Testing return of socket handle
    std::cout << "[Test] Initial socket handle state: " << test_socket.get_handle() << " (Expected -1)" << std::endl;

    // Testing passing of basic IPv4 TCP config
    if (!test_socket.create_socket(AF_INET, SOCK_STREAM, 0)){
        std::cerr << "FAILURE: Socket allocation failed." << std::endl;
        return 1;
    }

    std::cout << "[Test] Allocated socket handle: " << test_socket.get_handle() << " (Expected: file descriptor 3 or higher)" << std::endl;

    return 0;
}