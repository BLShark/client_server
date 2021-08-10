#pragma once
#include "dio.hpp"
#include "bbq.hpp"

class server: public dio{

private:
    sockaddr_in m_servAddr;
    int m_sockId;
    int m_port;
    int max_client = 2;
    std::list<std::thread> m_clientThreads;


private:
    void loop(int client)
    {
        auto push_notify = [&](std::string& answer) {
            char msg[1500];
            memset(&msg, 0, sizeof(msg)); // clear the buffer
            strcpy(msg, answer.c_str());
            send(client, (char *)&msg, strlen(msg), 0);

        };

        BBQ _bbq(push_notify);
        while (1)
        {
            //mutex
            bzero((char *)&m_servAddr, sizeof(m_servAddr));
            char msg[1500];
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            recv(client, (char *)&msg, sizeof(msg), 0);
            if (!strcmp(msg, "exit"))
            {
                std::cout << "Client has quit the session" << std::endl;
                return;
            }

            auto answer = _bbq.Request(std::move(std::string(msg)));
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, answer.c_str());
            send(client, (char*)&msg, strlen(msg), 0);

        }
    }

public:
    server(int port): m_port(port){}
    ~server()
    {
        close(m_sockId);
    }

    virtual bool Init(){
        //setup a socket and connection tools
        bzero((char*)&m_servAddr, sizeof(m_servAddr));
        m_servAddr.sin_family = AF_INET;
        m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        m_servAddr.sin_port = htons(m_port);


        m_sockId = socket(PF_INET, SOCK_STREAM, 0);
        if (m_sockId < 0 )
        {
            std::cout<<"server socket establishing error!"<<std::endl;
            return false;
        }

        int bind_status = bind(m_sockId, (struct sockaddr*)&m_servAddr, sizeof(m_servAddr));
        if (bind_status < 0 )
        {
            std::cout<<"binding socket to local address error!";
            return false;
        }

        if (ListenForConnection() == -1)
        {
            std::cout<<"error while trying to connect to listen to a client"<<std::endl;
            return false;
        }
        return true;
    }

    int  ListenForConnection(){
        return listen(m_sockId, 10);
    }

    virtual void Start()
    {
        while(max_client)
        {
            sockaddr_in newSockAddr;
            socklen_t newSockAddrSize = sizeof(newSockAddr);
            int client = accept(m_sockId, (sockaddr *)&newSockAddr, &newSockAddrSize);
            if (client)
            {
                 std::cout<<"Connected with client #:"<<client<<std::endl;
            }
            else
            {
                std::cout << "Error! Cannot established client connection"<< std::endl;
                continue;
            }
            m_clientThreads.emplace_back(std::thread(&server::loop, this ,client));
            max_client--;
        }
        for (auto& x: m_clientThreads)
        {
            std::cout<<"join threads"<<std::endl;
            x.join();
        }
    }


};
