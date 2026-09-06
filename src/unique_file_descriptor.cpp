#include <http/unique_file_descriptor.hpp>

#include <stdexcept>
#include <unistd.h>
#include <utility>

UniqueFileDescriptor::UniqueFileDescriptor(int fd)
{
    if (fd < INVALID_FD)
    {
        throw std::invalid_argument("Invalid file descriptor: must be nonnegative or -1.");
    }

    m_unique_fd = fd;
}

UniqueFileDescriptor::UniqueFileDescriptor(UniqueFileDescriptor&& other) noexcept
    : m_unique_fd(std::exchange(other.m_unique_fd, INVALID_FD)) { }

UniqueFileDescriptor& UniqueFileDescriptor::operator=(UniqueFileDescriptor&& other) noexcept
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
    if (m_unique_fd != INVALID_FD)
    {
        close(m_unique_fd);
        m_unique_fd = INVALID_FD;
    }
}

void UniqueFileDescriptor::reset(int fd)
{
    if (fd < INVALID_FD)
    {
        throw std::invalid_argument("Invalid file descriptor: must be nonnegative or -1.");
    }

    if (fd == m_unique_fd)
    {
        return;
    }

    reset();

    m_unique_fd = fd;
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
