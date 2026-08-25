#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

class WafSocket
{
private:
    int socket_handle;

public:
    // Lifecycle management
    WafSocket();
    ~WafSocket();

    // Create socket
    bool create_socket(int af, int socket_type, int protocol);

    // Bind socket to ip/port
    bool bind(const std::string& ip, int port);

    // Secure read-only getter window
    int get_handle() const;
};