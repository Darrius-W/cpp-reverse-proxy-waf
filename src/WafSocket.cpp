#include "WafSocket.h"

// Initialize descriptor to unallocated state in constructor
WafSocket::WafSocket(){
    socket_handle = -1; // -1 represents closed or invalid file descriptor
}

// Clean up data and avoid leaks with deconstructor
WafSocket::~WafSocket(){
    if (socket_handle != -1){
        ::close(socket_handle);
        std::cout << "[WafSocket] File descriptor " << socket_handle << " closed safely by deconstructor." << std::endl;
    }
}

// Return socket handle instance
int WafSocket::get_handle() const { return socket_handle; }

// Socket allocation
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