#ifndef TCP_HPP_
#define TCP_HPP_

#include "arpis_network/socket/socket.hpp"

namespace arpis_network {

class tcp : public socket {
public:
    tcp(const char * address, int port);
    int receive(char * buffer, int size);
    int send(char * buffer, int size);
    void set_max_con(int n);
    void serve();
protected:
    void listen();    
    void accept(int sockid);
private:
    int maxcon_;
};

} // arpis_network

#endif