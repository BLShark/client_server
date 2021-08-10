#pragma once

#include <netinet/in.h>
#include <strings.h> //bzero
#include <string.h>
#include <unistd.h> //close
#include <sys/types.h>   //bind
#include <sys/socket.h>
#include <thread>

class dio
{
public:
    virtual bool Init() = 0;
    virtual void Start() = 0;
};
