#include "platform.hpp"

#include <sys/socket.h>

namespace http_server::tcp
{

bool configure_connected_socket([[maybe_unused]] int fd)
{
#if defined(__linux__)
    // No configuration needed
    return true;
#elif defined(__APPLE__)
    constexpr int enabled = 1;
    return ::setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &enabled, sizeof(enabled)) == 0;
#else
#error "Unsupported platform"
#endif
}

} // namespace http_server::tcp
