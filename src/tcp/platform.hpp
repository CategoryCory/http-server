#pragma once

#include <sys/socket.h>

namespace http_server::tcp
{

#if defined(__linux__)
inline constexpr int TCP_SEND_FLAGS = MSG_NOSIGNAL;
#elif defined(__APPLE__)
inline constexpr int TCP_SEND_FLAGS = 0;
#else
#error "Unsupported platform"
#endif

/// @brief Applies platform-specific options to a connected client socket.
/// @param fd The connected socket's file descriptor.
/// @return true on success; otherwise false.
// TODO: Consider how to preserve failure errno
[[nodiscard]] bool configure_connected_socket(int fd);

} // namespace http_server::tcp
