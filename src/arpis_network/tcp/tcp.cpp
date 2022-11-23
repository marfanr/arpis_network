#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
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
    if (::listen(this->get_socket(), this->get_max_con()) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed listen tcp");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success listen tcp");
}

void tcp::accept(int sockid) {
    socklen_t s = sizeof(*this->get_sockaddr_in(sockid));
    if ((this->newsocket_ = ::accept(this->get_socket(), (sockaddr*)this->get_sockaddr_in(sockid), &s)) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed accept connection");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success accept connection");
}

void tcp::set_max_con(int n) {    
    this->maxcon_ = n;
}

int tcp::get_newsocket() {
    return this->newsocket_;
}

int tcp::get_max_con() {
    return this->maxcon_;
}

void tcp::serve() {
    int opt = 1;
    ::setsockopt(this->get_socket(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    this->bind(0);
    this->listen();
    RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "waiting client....");
    this->accept(1);
    RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "%s has joined server", inet_ntoa((*this->get_sockaddr_in(1)).sin_addr));
}

void tcp::connect() {
    if (::connect(this->get_socket(), (sockaddr*)this->get_sockaddr_in(0), sizeof(*this->get_sockaddr_in(0))) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed connect to server");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success connect to server");
}

void tcp::send(char * buffer, int buffsize) {    
    int socket = this->get_newsocket() == 0 ? this->get_socket() : this->get_newsocket();
    if (::send(socket, buffer, buffsize, 0) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed send message");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "sende");
} 
void tcp::receive(char * buffer, int buffsize) {
    int socket = this->get_newsocket() == 0 ? this->get_socket() : this->get_newsocket();
    ::read(socket, buffer, buffsize);     
}

void tcp::close() {
    if (::close(this->get_socket()))
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "server closed");
    if (::shutdown(this->get_socket(), SHUT_RDWR)) 
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "shutdown socket");
}

} // namespace arpis_network