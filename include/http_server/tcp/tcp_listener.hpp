#pragma once

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/socket_error.hpp>

#include <cstdint>
#include <expected>

namespace http_server::tcp
{

class TcpListener
{
public:
    [[nodiscard]] static std::expected<TcpListener, SocketError> create(std::uint16_t port, int max_backlog);

private:
    explicit TcpListener(core::UniqueFileDescriptor &&fd) noexcept;
    core::UniqueFileDescriptor m_fd;
};
} // namespace http_server::tcp
