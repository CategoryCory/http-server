# HTTP Server

A work-in-progress C++ portfolio project to build a simple, lightweight, and professional-grade HTTP server.

## Status

The project currently establishes the TCP server foundation. HTTP request handling and response generation are planned.

## Goals

- Demonstrate modern C++ design and resource management.
- Keep the server small, understandable, and dependency-light.
- Build toward a robust HTTP server with clear tests and maintainable code.

## Requirements

- CMake 3.20 or later
- Ninja
- Clang with C++23 support

## Build

```sh
cmake --preset clang-debug
cmake --build --preset clang-debug
```

## Test

```sh
ctest --preset clang-debug
```

## Roadmap

- [x] TCP socket lifecycle management
- [ ] Accept client connections
- [ ] Parse HTTP requests
- [ ] Generate HTTP responses
- [ ] Add request and response tests
- [ ] Document configuration and usage

## License

License information will be added before the project is released.