#include <iostream>
#include <memory>
#include "dio.hpp"
#include "server.hpp"
#include "client.hpp"


/* HOW TO START SERVER
 * the server support multiple clients!
 * the application has accepted list of params
 * @param [1] - "server"
 * @param [2] - {type port you want to open} ex. 12345
*/

/* HOW TO START CLIENT
 * the application has accepted list of params
 * @param [1] - "client"
 * @param [2] - {ip address}
 * @param [3] - {port}
*/



std::shared_ptr<dio> create_instance ( int argc, char *argv[])
{
    int port = std::stoi(argv[3]);
    if( std::string(argv[1]) == "server" && argc == 4 )
    {
        return std::make_shared<server>(port, argv[2]);
    }
    else if ( std::string(argv[1]) == "client" && argc == 4 )
    {
        return std::make_shared<client>(port, argv[2]);
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
