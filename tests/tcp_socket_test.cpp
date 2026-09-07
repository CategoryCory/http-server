#include <http/tcp_socket.hpp>

#include <arpa/inet.h>
#include <cassert>
#include <stdexcept>
#include <sys/socket.h>

namespace
{
void test_default_construction()
{
    const TcpSocket socket;

    assert(!socket.is_valid());
    assert(socket.get() == -1);
}

void test_initialization()
{
    TcpSocket socket;

    socket.initialize_socket();

    assert(socket.is_valid());
    assert(socket.get() >= 0);
}

void test_bind_requires_initialized_socket()
{
    TcpSocket socket;

    try
    {
        socket.bind_address(0);
        assert(false && "Expected std::logic_error");
    }
    catch (const std::logic_error &)
    {
    }
}

void test_invalid_port_is_rejected()
{
    TcpSocket socket;

    for (const int port : {-1, 65536})
    {
        try
        {
            socket.bind_address(port);
            assert(false && "Expected std::invalid_argument");
        }
        catch (const std::invalid_argument &)
        {
        }
    }
}

void test_bind_to_ephemeral_port()
{
    TcpSocket socket;
    socket.initialize_socket();
    socket.bind_address(0);

    sockaddr_in address{};
    socklen_t address_length = sizeof(address);
    assert(getsockname(socket.get(), reinterpret_cast<sockaddr *>(&address), &address_length) == 0);
    assert(address.sin_family == AF_INET);
    assert(ntohs(address.sin_port) != 0);
}
} // namespace

int main()
{
    test_default_construction();
    test_initialization();
    test_bind_requires_initialized_socket();
    test_invalid_port_is_rejected();
    test_bind_to_ephemeral_port();

    return 0;
}