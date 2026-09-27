#pragma once

#include <cstdint>

enum class TcpConnectionErrorCode : std::uint8_t
{
    ip_addr_conversion_failure,
};

struct TcpConnectionError
{
    TcpConnectionErrorCode code;
};
