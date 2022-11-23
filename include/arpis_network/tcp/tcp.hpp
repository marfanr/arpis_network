#ifndef TCP_HPP_
#define TCP_HPP_

#include "arpis_network/socket/socket.hpp"
#include <string>

namespace arpis_network {

class tcp : public socket {
public:
    tcp(const char * address, int port);        
    int get_max_con();
    void set_max_con(int n);
    void serve();
    void connect();
    void close();
    void send(char * buffer, int buffsize);    
    void receive(char * buffer, int buffsize);
protected:
    void listen();
    void accept(int sockid);
    int get_newsocket();    
private:
    int maxcon_;
    int newsocket_;
};

} // arpis_network

#endif