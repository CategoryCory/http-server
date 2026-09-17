#pragma once

#include <string>
#include <utility>

/// @brief Represents the success or failure of an operation.
///
/// A failed result carries an optional diagnostic message. Create instances
/// with success() or failure() to make the intended state explicit.
class [[nodiscard]] Result
{
  public:
    /// @brief Checks whether the operation completed successfully.
    /// @return true for a successful result; otherwise false.
    [[nodiscard]] bool is_success() const { return m_success; }

    /// @brief Checks whether the operation failed.
    /// @return true for a failed result; otherwise false.
    [[nodiscard]] bool is_failure() const { return !is_success(); }

    /// @brief Returns the diagnostic message associated with a failed result.
    /// @return The failure diagnostic, which may be empty.
    [[nodiscard]] const std::string &error_message() const { return m_error_message; }

    /// @brief Creates a successful result.
    /// @return A successful result.
    static Result success();

    /// @brief Creates a failed result.
    /// @param error_message A message describing the failure.
    /// @return A failed result.
    static Result failure(std::string error_message);

  private:
    Result(bool success, std::string error_message)
        : m_success(success), m_error_message(std::move(error_message))
    {
    }

    bool m_success;
    std::string m_error_message;
};