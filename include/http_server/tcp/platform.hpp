#pragma once

#include <sys/socket.h>

namespace http_server::tcp
{

#if defined(HTTP_SERVER_PLATFORM_LINUX)
constexpr int TCP_SEND_FLAGS = MSG_NOSIGNAL;
#elif defined(HTTP_SERVER_PLATFORM_MACOS)
constexpr int TCP_SEND_FLAGS = 0;
#else
#error "Unsupported platform"
#endif

// TODO: Consider how to preserve failure errno
inline bool configure_connected_socket(int fd)
{
#if defined(HTTP_SERVER_PLATFORM_LINUX)
    // No configuration needed
    return true;
#elif defined(HTTP_SERVER_PLATFORM_MACOS)
    constexpr int enabled = 1;
    return ::setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &enabled, sizeof(enabled)) == 0;
#else
#error "Unsupported platform"
#endif
}

} // namespace http_server::tcp
