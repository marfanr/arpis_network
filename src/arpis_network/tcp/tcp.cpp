#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include "arpis_network/tcp/tcp.hpp"
#include "rclcpp/rclcpp.hpp"


namespace arpis_network {

tcp::tcp(const char * address, int port) : socket(AF_INET, SOCK_STREAM, 0) {    
    sockaddr_in sock1, sock2;    
    bzero(&sock1, sizeof(sock1));
    sock1.sin_addr.s_addr = inet_addr(address);
    sock1.sin_family = AF_INET;
    sock1.sin_port = htons((uint16_t)port);
    this->register_sockaddr_in(&sock1);   
    this->register_sockaddr_in(&sock2);   
}

void tcp::listen() {
    if (::listen(this->get_socket(), this->maxcon_) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed listen tcp");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success listen tcp");
}

void tcp::accept(int sockid) {
    socklen_t s = sizeof(*this->get_sockaddr_in(sockid));
    if (::accept(this->get_socket(), (sockaddr*)this->get_sockaddr_in(sockid), &s) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed accept connection");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success accept connection");
}

void tcp::set_max_con(int n) {
    this->maxcon_ = n;
}

void tcp::serve() {
    if (::bind(this->get_socket(), (sockaddr*)this->get_sockaddr_in(0), sizeof(*this->get_sockaddr_in(0))) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed bind socket");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success bind socket");
        
    this->listen();
    RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "waiting client....");
    this->accept(1);
}

} // namespace arpis_network