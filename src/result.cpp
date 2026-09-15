#include <http/result.hpp>

Result Result::success(std::string message) {
    return Result(true, std::move(message));
}

Result Result::failure(std::string error_message) {
    return Result(false, std::move(error_message));
}