# HTTP Server

A work-in-progress C++ portfolio project to build a simple, lightweight, and professional-grade HTTP server.

## Status

The project currently establishes the TCP server foundation. HTTP request handling and response generation are planned.

## Goals

- Demonstrate modern C++ design and resource management.
- Keep the server small, understandable, and dependency-light.
- Build toward a robust HTTP server with clear tests and maintainable code.

## Platform Support

The server currently targets POSIX systems: macOS and Linux. Windows is not
supported yet because the networking implementation uses POSIX sockets and file
descriptors.

## Install and Run

### Prerequisites

- CMake 3.20 or later
- Ninja
- A C++23-capable compiler, such as AppleClang, Clang, or GCC
- [vcpkg](https://github.com/microsoft/vcpkg)

Install CMake, Ninja, and a compiler using your platform's package manager. For
example:

```sh
# macOS
brew install cmake ninja llvm

# Ubuntu
sudo apt update
sudo apt install build-essential cmake ninja-build
```

Clone and bootstrap vcpkg, then set `VCPKG_ROOT` to its installation directory:

```sh
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh"
export VCPKG_ROOT="$HOME/vcpkg"
```

The project uses vcpkg manifest mode for test dependencies and pins its registry
baseline in `vcpkg-configuration.json`.

### Configure, Build, and Test

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

The first configuration installs manifest dependencies automatically. Use the
`release` preset for an optimized build without tests.

When switching an existing build directory to a different compiler or vcpkg
toolchain, remove that build directory before configuring again:

```sh
rm -rf build/debug
```

### Select a Compiler

The committed presets intentionally do not choose a compiler. CMake uses the
default compiler from your environment, so both VS Code and CLion can consume
the same project configuration without editor-specific settings.

For a one-time compiler selection, configure with `CC` and `CXX`:

```sh
CC=clang CXX=clang++ cmake --preset debug
```

For a persistent local choice, create an ignored `CMakeUserPresets.json`:

```json
{
	"version": 2,
	"configurePresets": [
		{
			"name": "local-clang",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_COMPILER": "/opt/homebrew/opt/llvm/bin/clang++"
			}
		},
		{
			"name": "clang-debug",
			"inherits": ["debug", "local-clang"]
		}
	],
	"buildPresets": [
		{
			"name": "clang-debug",
			"configurePreset": "clang-debug"
		}
	],
	"testPresets": [
		{
			"name": "clang-debug",
			"configurePreset": "clang-debug",
			"output": {
				"outputOnFailure": true
			}
		}
	]
}
```

This file is ignored by Git and can contain compiler paths or other
machine-specific configuration. Use the equivalent compiler path on Ubuntu when
needed.

### Code Quality Tools

With `clang-format` and `clang-tidy` available on `PATH`, format the project
with:

```sh
find include src tests -type f \( -name '*.cpp' -o -name '*.hpp' \) -exec clang-format -i {} +
```

After configuring the Debug preset, run clang-tidy against the compilation
database:

```sh
clang-tidy -p build/debug --config-file=.clang-tidy src/*.cpp tests/*.cpp
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