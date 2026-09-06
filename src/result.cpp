#include <http/result.hpp>

Result Result::success(const std::string& message) {
    return Result(true, message);
}

Result Result::failure(const std::string& error_message) {
    return Result(false, error_message);
}