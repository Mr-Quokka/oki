# Packaging an existing project

You may want to create a package from an existing library.
In this cookbook, we will explore how to package the [`yaml-cpp`](https://github.com/jbeder/yaml-cpp) library.

## Retrieve the library

In our case, `yaml-cpp` can be obtained via a Git repository, so we will clone it.

```bash
git clone --depth 1 https://github.com/jbeder/yaml-cpp
```

`yaml-cpp` uses [CMake](https://cmake.org) to build itself.
The build command should invoke the `cmake` command so.

If we explore the repository, we can see that the header files are separated from the source files, so we need to keep that in mind when specifying the paths to include in the package:

```console
$ ls
appveyor.yml              include      util
BUILD.bazel               install.txt  WORKSPACE
CMakeLists.txt            LICENSE      yaml-cpp-config.cmake.in
cmake_uninstall.cmake.in  README.md    yaml-cpp.pc.in
CONTRIBUTING.md           src
docs                      test
```

## Write a manifest

You can generate a sample `oki.toml` file with `oki init cpp --lib` that we will further edit.

We add a `[lib.c]` section to explain in which directory header files are located, how to statically build and link the library.

```toml
[package]
name = "yaml-cpp"
description = "A YAML parser and emitter in C++"
version = "0.7.0"
kind = "cpp"
include = [
    "CMakeLists.txt",
    "include",
    "src",
    "util",
    "yaml-cpp.pc.in",
    "yaml-cpp-config.cmake.in",
    "cmake_uninstall.cmake.in"
]

[lib.c]
export = "include"
build-static = "mkdir build && cd build && cmake .. && make"
static-link = "build/libyaml-cpp.a"
```

## Publish

Try to publish it in the local registry to test if the package is correctly configured, then if everything works, you can publish it to a real repository:

```bash
oki publish --registry local
# oki publish
```

## Test

Let's test this new package in a real project!

```console
$ cd /tmp && mkdir yaml-cpp-test
$ cd yaml-cpp-test && oki init c
Created `yaml-cpp-test` package
```

Add the dependency:

```console
$ oki install yaml-cpp
 + yaml-cpp 0.7.0
Installed 1 package
```

Write a code that tests if everything compile:

```cpp
// src/main.cpp
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

int main() {
    YAML::Node config = YAML::Load("username: john");
    const std::string username = config["username"].as<std::string>();
    std::cout << "Username: " << username << "\n";
    return 0;
}
```

```console
$ oki makefile
Generated Makefile
$ make
mkdir build
g++ -isystemoki-packages/yaml-cpp/include -std=c++20 -Wall -Wextra -pedantic -g -MMD -MP -c src/main.cpp -o build/main.o
oki build
make[1]: Entering directory '/tmp/yaml-cpp-test'
cd oki-packages/yaml-cpp && mkdir build && cd build && cmake .. && make
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /tmp/yaml-cpp-test/oki-packages/yaml-cpp/build
Scanning dependencies of target yaml-cpp
[  1%] Building CXX object CMakeFiles/yaml-cpp.dir/src/contrib/graphbuilder.cpp.o
[...]
[100%] Built target yaml-cpp
make[1]: Leaving directory '/tmp/yaml-cpp-test'
g++ -o yaml-cpp-test build/main.o oki-packages/yaml-cpp/build/libyaml-cpp.a
```

Success!

```console
$ ./yaml-cpp-test 
Username: john
```
