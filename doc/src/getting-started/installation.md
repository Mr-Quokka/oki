# Installation

## Build and Install Oki from Source

Oki requires a C++ 20 compiler. GCC 10 or Clang 12 (or later) are recommended.

### Install dependencies

Oki depends on the [nlohmann/json](https://github.com/nlohmann/json),
[curl](https://github.com/curl/curl),
[toml++](https://github.com/marzer/tomlplusplus),
[boost::program_options](https://github.com/boostorg/program_options),
and [minizip](https://github.com/madler/zlib/tree/master/contrib/minizip) libraries.

On Debian:
```bash
apt install build-essential nlohmann-json3-dev libcurl4-openssl-dev libminizip-dev libboost-program-options-dev doctest-dev
```

On Arch Linux:
```bash
pacman -Sy nlohmann-json curl tomlplusplus minizip boost doctest
```

Run the `configure.sh` script with the `-d` option to check if all dependencies are found.
This script may use the `third-party` folder to symlink or download header-only dependencies.

## Build

Execute `make` in the `cli` folder.

```bash
make -j$(nproc) BUILD=release
```

## Install

Oki can be installed in any directory.
By default, it will be installed in `/usr/local/bin` but you can change this behavior by overriding the [installation directories variables](https://www.gnu.org/prep/standards/html_node/Directory-Variables.html#Directory-Variables).

```bash
make install BUILD=release
```
