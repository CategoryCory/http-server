#include <http/tcp_server.hpp>

Result TcpServer::start(const TcpServerConfig& server_config)
{
    try
    {
        m_socket.initialize_socket();
        m_socket.bind_address(server_config.port);
        m_socket.listen_for_connections(server_config.max_backlog);
    }
    catch(const std::exception& e)
    {
        return Result::failure(e.what());
    }

    return Result::success();
}

void TcpServer::stop()
{
    throw std::runtime_error("TcpServer::stop() not yet implemented");
}
