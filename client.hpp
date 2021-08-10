#pragma once
#include <atomic>
#include <netdb.h>
#include <arpa/inet.h>
#include "dio.hpp"

class client : public dio
{
    sockaddr_in sendSockAddr;
    char* m_serverIp = 0;
    int m_port = 0;
    int m_clientSd = 0;
//    std::thread m_receiveMsgThread;
    std::atomic<bool> m_threadExit;

private:
    inline void loop(){

        std::thread receive_t ([&](){
            char rcv_msg[1000];
            while(1){
                if (m_threadExit) break;

               recv(m_clientSd, (char*)&rcv_msg, sizeof(rcv_msg), 0);
               if(!strcmp(rcv_msg, "CLOSED BYE!"))
               {
                   std::cout << "The restaurant is closed. \n Server has quit the session" << std::endl;
                   break;
               }
               std::cout<<"Server: "<<std::endl;
            }
        });

        char msg[1000];
        while(1){
            std::cout << ">";
            std::string data;
            getline(std::cin, data);
            memset(&msg, 0, sizeof(msg));//clear the buffer
            strcpy(msg, data.c_str());
            if(data == "exit")
            {
               send(m_clientSd, (char*)&msg, strlen(msg), 0);
               break;
            }
            send(m_clientSd, (char*)&msg, strlen(msg), 0);
        }

        receive_t.join();
    }

public:
    client(char *serverIp, int port ) {
        m_serverIp = serverIp;
        m_port = port;
    }
    ~client(){}

    inline virtual bool Init(){
        struct hostent *host = gethostbyname(m_serverIp);
        bzero((char *)&sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
        sendSockAddr.sin_port = htons(m_port);
        m_clientSd = socket(AF_INET, SOCK_STREAM, 0);
    }

    inline virtual void Start() {
        int status = connect(m_clientSd,
                                 (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if(status < 0)
        {
            std::cout<<"Error connecting to socket!"<<std::endl;
            return;
        }
        std::cout << "Connected to the server!" <<std::endl;

        loop();
    }



};
