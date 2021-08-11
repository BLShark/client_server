#pragma once
#include <atomic>
#include "dio.hpp"

class client : public dio
{
    sockaddr_in sendSockAddr;
    struct hostent *host;
    int m_clientSd = 0;
    std::atomic<bool> m_threadExit;

private:
    inline void loop(){

        std::thread receive_t ([&](){
            char rcv_msg[1000];
            while(1){
                if (m_threadExit) break;
               memset(&rcv_msg, 0, sizeof(rcv_msg)); //clear the buffer
               recv(m_clientSd, (char*)&rcv_msg, sizeof(rcv_msg), 0);
               if(!strcmp(rcv_msg, "SERVED BYE!"))
               {
                   std::cout << "The restaurant is closed. \n Server has quit "
                                "the session. \n Type exit to leave the restaurant."
                             << std::endl;
                   break;
               }
               std::cout<<"Server: "<<rcv_msg<<std::endl;
            }
        });

        char msg[1000];
        while(1){
            std::cerr << ">";
            std::string data;
            getline(std::cin, data);
            memset(&msg, 0, sizeof(msg));//clear the buffer
            strcpy(msg, data.c_str());
            if(data == "exit")
            {
               send(m_clientSd, (char*)&msg, strlen(msg), 0);
               m_threadExit = true;
               break;
            }
            send(m_clientSd, (char*)&msg, strlen(msg), 0);
        }
        std::cout<<"exiting the program. wait receiving thread to join.";
        receive_t.join();
        std::cout<<"thread has joined."<<std::endl;
    }

public:
    client(int port, char *serverIp ) : dio(port, serverIp), m_threadExit(false) {

    }
    ~client(){}

     virtual bool Init(){
        host = gethostbyname(m_serverIp);
        bzero((char *)&sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
        sendSockAddr.sin_port = htons(m_port);
        m_clientSd = socket(AF_INET, SOCK_STREAM, 0);
        return true;
    }

     virtual void Start() {
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
