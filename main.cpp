#include <iostream>
#include <memory>
#include "dio.hpp"
#include "server.hpp"
#include "client.hpp"


std::shared_ptr<dio> create_instance ( int argc, char *argv[])
{
    if( std::string(argv[1]) == "server" && argc == 2 )
    {
        int port = std::stoi(argv[2]);
        return std::make_shared<server>(port);
    }
    else if ( std::string(argv[1]) == "client" && argc == 3 )
    {
        int port = std::stoi(argv[3]);
        return std::make_shared<client>(argv[2], port);
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    auto _dio = create_instance(argc, argv);
    if( _dio )
    {
        _dio.get()->Init();
        _dio.get()->Start();
    }

    return 0;
}
