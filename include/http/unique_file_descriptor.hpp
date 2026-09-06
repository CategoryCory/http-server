#pragma once

/// @brief RAII owner for a POSIX file descriptor.
///
/// Ensures an owned descriptor is closed when this object is destroyed or
/// replaced. Instances cannot be copied but may be moved to transfer ownership.
class UniqueFileDescriptor
{
public:
    /// @brief Creates an empty descriptor owner.
    UniqueFileDescriptor() = default;

    /// @brief Creates an owner for a file descriptor.
    /// @param fd The descriptor to own, or -1 for an empty owner.
    /// @throws std::invalid_argument if @p fd is less than -1.
    explicit UniqueFileDescriptor(int fd);

    /// @brief Copying is disabled because descriptors have unique ownership.
    UniqueFileDescriptor(const UniqueFileDescriptor&) = delete;

    /// @brief Copy assignment is disabled because descriptors have unique ownership.
    UniqueFileDescriptor& operator=(const UniqueFileDescriptor&) = delete;

    /// @brief Transfers descriptor ownership from another instance.
    /// @param other The instance to move from; it becomes empty.
    UniqueFileDescriptor(UniqueFileDescriptor&&) noexcept;

    /// @brief Replaces this object's descriptor with one owned by another instance.
    /// @param other The instance to move from; it becomes empty.
    /// @return A reference to this object.
    UniqueFileDescriptor& operator=(UniqueFileDescriptor&&) noexcept;

    /// @brief Closes the owned descriptor, if any.
    ~UniqueFileDescriptor();

    /// @brief Closes the owned descriptor and leaves this object empty.
    void reset() noexcept;

    /// @brief Replaces the owned descriptor.
    /// @param fd The descriptor to own, or -1 for an empty owner.
    /// @throws std::invalid_argument if @p fd is less than -1.
    void reset(int fd);

    /// @brief Releases ownership without closing the descriptor.
    /// @return The previously owned descriptor, or -1 if this object was empty.
    [[nodiscard]] int release() noexcept;

    /// @brief Checks whether this object owns a valid descriptor.
    /// @return true when a descriptor is owned; otherwise false.
    [[nodiscard]] bool is_valid() const noexcept;

    /// @brief Checks whether this object owns a valid descriptor.
    /// @return true when a descriptor is owned; otherwise false.
    [[nodiscard]] explicit operator bool() const noexcept;

    /// @brief Returns the currently owned descriptor without transferring ownership.
    /// @return The owned descriptor, or -1 if this object is empty.
    [[nodiscard]] int get() const noexcept;
private:
    static constexpr int INVALID_FD = -1;
    int m_unique_fd = INVALID_FD;
};