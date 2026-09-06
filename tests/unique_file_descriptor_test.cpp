#include <http/unique_file_descriptor.hpp>

#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <utility>

namespace
{
int create_file_descriptor()
{
    int descriptors[2]{};
    assert(pipe(descriptors) == 0);
    assert(close(descriptors[1]) == 0);
    return descriptors[0];
}

bool is_closed(int fd)
{
    errno = 0;
    return fcntl(fd, F_GETFD) == -1 && errno == EBADF;
}

void test_default_construction()
{
    UniqueFileDescriptor descriptor;

    assert(!descriptor.is_valid());
    assert(!descriptor);
    assert(descriptor.get() == -1);
    assert(descriptor.release() == -1);
    descriptor.reset();
}

void test_empty_descriptor_construction()
{
    UniqueFileDescriptor descriptor(-1);

    assert(!descriptor.is_valid());
    assert(descriptor.get() == -1);
}

void test_invalid_descriptor_rejected()
{
    try
    {
        [[maybe_unused]] UniqueFileDescriptor descriptor(-2);
        assert(false && "Expected std::invalid_argument");
    }
    catch (const std::invalid_argument &)
    {
    }
}

void test_destruction_closes_descriptor()
{
    const int fd = create_file_descriptor();
    {
        UniqueFileDescriptor descriptor(fd);
        assert(descriptor.is_valid());
        assert(descriptor);
        assert(descriptor.get() == fd);
    }

    assert(is_closed(fd));
}

void test_reset_closes_and_replaces_descriptor()
{
    const int old_fd = create_file_descriptor();
    const int new_fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(old_fd);

    descriptor.reset(new_fd);

    assert(is_closed(old_fd));
    assert(descriptor.get() == new_fd);
    assert(descriptor.is_valid());
}

void test_reset_to_empty_closes_descriptor()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(fd);

    descriptor.reset(-1);

    assert(is_closed(fd));
    assert(!descriptor.is_valid());
    assert(descriptor.get() == -1);
}

void test_reset_same_descriptor_preserves_ownership()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(fd);

    descriptor.reset(fd);

    assert(descriptor.get() == fd);
    assert(fcntl(fd, F_GETFD) != -1);
}

void test_invalid_reset_preserves_current_descriptor()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(fd);

    try
    {
        descriptor.reset(-2);
        assert(false && "Expected std::invalid_argument");
    }
    catch (const std::invalid_argument &)
    {
    }

    assert(descriptor.get() == fd);
    assert(fcntl(fd, F_GETFD) != -1);
}

void test_release_transfers_ownership()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(fd);

    const int released_fd = descriptor.release();

    assert(released_fd == fd);
    assert(!descriptor.is_valid());
    assert(fcntl(released_fd, F_GETFD) != -1);
    assert(close(released_fd) == 0);
}

void test_move_construction_transfers_ownership()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor source(fd);

    UniqueFileDescriptor destination(std::move(source));

    assert(destination.get() == fd);
    assert(destination.is_valid());
    // NOLINTNEXTLINE(bugprone-use-after-move): verifies the documented moved-from state.
    assert(!source.is_valid());
}

void test_move_assignment_releases_previous_descriptor()
{
    const int source_fd = create_file_descriptor();
    const int destination_fd = create_file_descriptor();
    UniqueFileDescriptor source(source_fd);
    UniqueFileDescriptor destination(destination_fd);

    destination = std::move(source);

    assert(is_closed(destination_fd));
    assert(destination.get() == source_fd);
    // NOLINTNEXTLINE(bugprone-use-after-move): verifies the documented moved-from state.
    assert(!source.is_valid());
}

void test_self_move_assignment_preserves_descriptor()
{
    const int fd = create_file_descriptor();
    UniqueFileDescriptor descriptor(fd);
    UniqueFileDescriptor &same_descriptor = descriptor;

    descriptor = std::move(same_descriptor);

    assert(descriptor.get() == fd);
    assert(descriptor.is_valid());
    assert(fcntl(fd, F_GETFD) != -1);
}
} // namespace

int main()
{
    test_default_construction();
    test_empty_descriptor_construction();
    test_invalid_descriptor_rejected();
    test_destruction_closes_descriptor();
    test_reset_closes_and_replaces_descriptor();
    test_reset_to_empty_closes_descriptor();
    test_reset_same_descriptor_preserves_ownership();
    test_invalid_reset_preserves_current_descriptor();
    test_release_transfers_ownership();
    test_move_construction_transfers_ownership();
    test_move_assignment_releases_previous_descriptor();
    test_self_move_assignment_preserves_descriptor();

    return 0;
}