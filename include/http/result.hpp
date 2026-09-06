#pragma once

#include <string>

class Result
{
public:
    Result() = default;

    Result(bool success, const std::string& error_message)
        : m_success(success), m_error_message(error_message) {}

    [[nodiscard]] bool is_success() const { return m_success; }
    [[nodiscard]] const std::string& error_message() const { return m_error_message; }

    static Result success(const std::string& message = "");
    static Result failure(const std::string& error_message);
private:
    bool m_success = false;
    std::string m_error_message{};
};