#include <http/result.hpp>

#include <cassert>
#include <string>

namespace
{
void test_default_result_is_failure()
{
    const Result result;

    assert(!result.is_success());
    assert(result.message().empty());
}

void test_success_without_message()
{
    const Result result = Result::success();

    assert(result.is_success());
    assert(result.message().empty());
}

void test_success_preserves_message()
{
    constexpr std::string message = "Request completed";
    const Result result = Result::success(message);

    assert(result.is_success());
    assert(result.message() == message);
}

void test_failure_preserves_message()
{
    constexpr std::string message = "Unable to bind socket";
    const Result result = Result::failure(message);

    assert(!result.is_success());
    assert(result.message() == message);
}
} // namespace

int main()
{
    test_default_result_is_failure();
    test_success_without_message();
    test_success_preserves_message();
    test_failure_preserves_message();

    return 0;
}