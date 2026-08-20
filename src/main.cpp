#include <iostream>
#include <socket.h>
#pragma comment(lib, "ws2_32.lib")

int main(){
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);

    std::cout << "WAF starting ..." << std::endl;

    return 0;
}