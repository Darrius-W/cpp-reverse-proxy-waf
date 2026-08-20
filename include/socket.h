#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>

class Socket
{
private:
    // socket declaration
    SOCKET socket_handle;
public:
    // create socket
    bool create_socket(int af, int socket_type, int protocol);
    // bind socket to ip/port
    bool bind(std::string ip, int port);
    // listen for a connection
    bool listen(int backlog);
    // accept connection
    SOCKET accept_conn();
    // send data
    int send_data();
    // receive data
    int receive_data();
    // close socket
    bool close_socket();
};