# Change Log
All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased] - 2026-09-19

### Added

- POSIX TCP socket lifecycle management with RAII file descriptor ownership.
- TCP server startup, shutdown, restart, and client connection acceptance.
- TOML configuration loading and validation for the TCP port and backlog.
- Debug build and test presets using CMake, Ninja, and vcpkg.
- Unit and loopback integration tests for configuration, sockets, server state,
  client acceptance, and file descriptor ownership.

### Changed

- Documented the currently supported TCP foundation and clarified that HTTP
  request processing and the long-running server loop are still planned.