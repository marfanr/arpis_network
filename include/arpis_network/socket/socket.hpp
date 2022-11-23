#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <netinet/in.h>
#include <vector>

namespace arpis_network {

class socket {
public:
    socket(int domain, int type, int protocol);    
    void register_sockaddr_in(sockaddr_in * sockaddrin);
    sockaddr_in * get_sockaddr_in(int id);
    int get_socket();
    void bind(int sockid);
protected:
private:
    int socket_;
    std::vector<sockaddr_in * > sockaddr_in_;
};

} // namespace arpis_network

#endif