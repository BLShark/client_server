#pragma once

#include <netinet/in.h>
#include <strings.h> //bzero
#include <string.h>
#include <unistd.h> //close
#include <sys/types.h>   //bind
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>

class dio
{
protected:
    char* m_serverIp = 0;
    int m_port = 0;

public:
    dio(int port, char *serverIp): m_port(port) {
        m_serverIp = serverIp;
    }
    virtual ~dio(){}

    virtual bool Init() = 0;
    virtual void Start() = 0;
};
