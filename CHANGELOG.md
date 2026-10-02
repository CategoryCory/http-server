# Change Log
All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased] - 2026-09-19

### Added

- POSIX TCP socket lifecycle management with RAII file descriptor ownership.
- TCP server startup, shutdown, restart, and client connection acceptance.
- RAII-managed TCP client connections with peer IP address and port access.
- Connection data transmission that retries interrupted writes and reports
  typed failures when no progress or a system error occurs.
- TOML configuration loading and validation for the TCP port and backlog.
- Debug build and test presets using CMake, Ninja, and vcpkg.
- Unit and loopback integration tests for configuration, sockets, server state,
  client acceptance, connection metadata, and file descriptor ownership.

### Changed

- Reorganized the project into `core`, `tcp`, and `config` layers with matching
  `include/http_server/`, `src/`, and `tests/` directories, `http_server::*`
  namespaces, and one CMake library and test executable per layer.
- Moved `TcpServerConfig` into the `tcp` layer so the transport layer does not
  depend on configuration loading.
- Made the platform-specific socket helpers private to the `tcp` layer and
  detect the platform with compiler macros instead of CMake definitions.
- Removed ad-hoc checks from the command-line program; they are covered by the
  `TcpServer` unit tests.
- Updated the clang-tidy tasks and documentation to find sources recursively.
- Documented the currently supported TCP foundation and clarified that HTTP
  request processing and the long-running server loop are still planned.
- Updated the command-line program to accept one client connection before it
  stops; persistent connection processing remains planned.