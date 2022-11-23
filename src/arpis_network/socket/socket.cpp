#include <cstring>
#include <sys/socket.h>
#include "arpis_network/socket/socket.hpp"
#include "rclcpp/rclcpp.hpp"

namespace arpis_network {

socket::socket(int domain, int type, int protocol) {
    if ((this->socket_ = ::socket(domain, type, protocol)) < 0) {
        RCLCPP_ERROR(rclcpp::get_logger("arps_network/socket"), "failied create socket");
    } else
        RCLCPP_INFO(rclcpp::get_logger("arps_network/socket"), "success create socket");
}

void socket::register_sockaddr_in(sockaddr_in * sockaddrin) {
    this->sockaddr_in_.push_back(sockaddrin);
}

sockaddr_in * socket::get_sockaddr_in(int id) {
    return this->sockaddr_in_.at(id);
}

int socket::get_socket() {
    return this->socket_;
}

void socket::bind(int sockid) {
    if (::bind(this->get_socket(), (sockaddr*)this->get_sockaddr_in(sockid), sizeof(*this->get_sockaddr_in(sockid))) < 0)
        RCLCPP_ERROR(rclcpp::get_logger("arpis_network/tcp"), "failed bind socket");
    else
        RCLCPP_INFO(rclcpp::get_logger("arpis_network/tcp"), "success bind socket");    
}

} // namespace arpis_network