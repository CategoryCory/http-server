#include <http/result.hpp>

Result Result::success() { return Result(true, {}); }

Result Result::failure(std::string error_message) { return Result(false, std::move(error_message)); }