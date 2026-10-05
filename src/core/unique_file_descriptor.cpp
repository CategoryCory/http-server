#include <http_server/core/unique_file_descriptor.hpp>

#include <cassert>
#include <cerrno>
#include <stdexcept>
#include <unistd.h>
#include <utility>

namespace http_server::core
{

UniqueFileDescriptor::UniqueFileDescriptor(int fd)
{
    if (fd < INVALID_FD)
    {
        throw std::invalid_argument("Invalid file descriptor: must be nonnegative or -1.");
    }

    m_unique_fd = fd;
}

UniqueFileDescriptor::UniqueFileDescriptor(UniqueFileDescriptor &&other) noexcept
    : m_unique_fd(std::exchange(other.m_unique_fd, INVALID_FD))
{
}

UniqueFileDescriptor &UniqueFileDescriptor::operator=(UniqueFileDescriptor &&other) noexcept
{
    if (this != &other)
    {
        reset();
        m_unique_fd = std::exchange(other.m_unique_fd, INVALID_FD);
    }
    return *this;
}

UniqueFileDescriptor::~UniqueFileDescriptor()
{
    reset();
}

void UniqueFileDescriptor::reset() noexcept
{
    const auto original_error = errno;

    if (m_unique_fd != INVALID_FD)
    {
        const auto fd_to_close = std::exchange(m_unique_fd, INVALID_FD);

        if (::close(fd_to_close) == -1)
        {
            [[maybe_unused]] const auto close_error = errno;
            assert(close_error != EBADF);
        }
    }

    errno = original_error;
}

int UniqueFileDescriptor::get() const noexcept
{
    return m_unique_fd;
}

int UniqueFileDescriptor::release() noexcept
{
    return std::exchange(m_unique_fd, INVALID_FD);
}

bool UniqueFileDescriptor::is_valid() const noexcept
{
    return m_unique_fd != INVALID_FD;
}

UniqueFileDescriptor::operator bool() const noexcept
{
    return is_valid();
}

} // namespace http_server::core
