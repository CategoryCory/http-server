#pragma once

#include <string>

/// @brief Represents the success or failure of an operation.
///
/// A result optionally carries a message describing the outcome. Create
/// instances with success() or failure() to make the intended state explicit.
class Result
{
public:
    /// @brief Creates a failed result with no message.
    Result() = default;

    /// @brief Checks whether the operation completed successfully.
    /// @return true for a successful result; otherwise false.
    [[nodiscard]] bool is_success() const { return m_success; }

    /// @brief Returns the message associated with this result.
    /// @return The outcome message, which may be empty.
    [[nodiscard]] const std::string& message() const { return m_message; }

    /// @brief Creates a successful result.
    /// @param message An optional message describing the successful outcome.
    /// @return A successful result.
    static Result success(const std::string& message = "");

    /// @brief Creates a failed result.
    /// @param message A message describing the failure.
    /// @return A failed result.
    static Result failure(const std::string& message);
private:
    Result(bool success, const std::string& message)
        : m_success(success), m_message(message) {}

    bool m_success = false;
    std::string m_message{};
};