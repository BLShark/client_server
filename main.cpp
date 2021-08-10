#include <iostream>
#include <memory>
#include "dio.hpp"
#include "server.hpp"
#include "client.hpp"


std::shared_ptr<dio> create_instance (char *argv[])
{
    if(argv[1] == "server")
    {
        return std::move(std::make_shared<server>(argv[2]));
    }
    else if (argv[1] == "client")
    {
        return std::move(std::make_shared<client>(argv[2], argv[3]));
    }
}

int main(int argc, char *argv[])
{

    std::shared_ptr<dio> _dio = create_instance(argv);
    _dio.get()->Init();
    _dio.get()->Start();

    return 0;
}
