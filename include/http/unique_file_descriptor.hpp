#pragma once

class UniqueFileDescriptor
{
public:
    UniqueFileDescriptor() = default;
    explicit UniqueFileDescriptor(int fd);

    UniqueFileDescriptor(const UniqueFileDescriptor&) = delete;
    UniqueFileDescriptor& operator=(const UniqueFileDescriptor&) = delete;

    UniqueFileDescriptor(UniqueFileDescriptor&&) noexcept;
    UniqueFileDescriptor& operator=(UniqueFileDescriptor&&) noexcept;

    ~UniqueFileDescriptor();

    void reset() noexcept;
    void reset(int fd);

    [[nodiscard]] int release() noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
    [[nodiscard]] explicit operator bool() const noexcept;
    [[nodiscard]] int get() const noexcept;
private:
    static constexpr int INVALID_FD = -1;
    int m_unique_fd = INVALID_FD;
};