#include <http/tcp_server.hpp>

Result TcpServer::start(std::uint16_t port, int max_backlog)
{
    try
    {
        m_socket.initialize_socket();
        m_socket.bind_address(port);
        m_socket.listen_for_connections(max_backlog);
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
