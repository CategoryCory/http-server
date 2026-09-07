#pragma once

#include <http/unique_file_descriptor.hpp>

class Socket
{
  public:
    Socket() = default;

    explicit Socket(int fd);

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    Socket(Socket &&other) noexcept;

    Socket &operator=(Socket &&other) noexcept;

    ~Socket();

    [[nodiscard]] bool is_valid() const noexcept;

    [[nodiscard]] int get() const noexcept;

  private:
    UniqueFileDescriptor m_socket_fd{};
};
