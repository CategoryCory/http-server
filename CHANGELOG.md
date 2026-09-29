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

- Documented the currently supported TCP foundation and clarified that HTTP
  request processing and the long-running server loop are still planned.
- Updated the command-line program to accept one client connection before it
  stops; persistent connection processing remains planned.