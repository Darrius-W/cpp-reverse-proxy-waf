#include "WafSocket.h"

// Initialize descriptor to unallocated state
WafSocket::WafSocket(){
    socket_handle = -1; // -1 represents closed or invalid file descriptor
}

// Clean up data to avoid leaks
WafSocket::~WafSocket(){
    if (socket_handle != -1){
        std::cout << "[WafSocket] File descriptor " << socket_handle << " closing safely by deconstructor." << std::endl;
        ::close(socket_handle);
    }
}

// Return socket handle in safe read-only state 
int WafSocket::get_handle() const { return socket_handle; }

// Initialize a new network endpoint (socket) with the OS
bool WafSocket::create_socket(int af, int socket_type, int protocol){
    // if instance already managing active pipeline, close it
    if (socket_handle != -1){
        ::close(socket_handle);
    }

    socket_handle = ::socket(af, socket_type, protocol);

    // if socket doesnt get assigned properly, display error & end
    if (socket_handle == -1){
        std::cerr << "[WafSocket] Error: Socket creation failed. System Errno: " << errno << std::endl;
        return false;
    }

    std::cout << "[WafSocket] Socket created successfully on file descriptor: " << socket_handle << std::endl;
    return true;
}