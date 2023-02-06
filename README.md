<pre>
 ██████╗ ██╗  ██╗██╗
██╔═══██╗██║ ██╔╝██║
██║   ██║█████╔╝ ██║
██║   ██║██╔═██╗ ██║
╚██████╔╝██║  ██╗██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝
</pre>

Open pacKage Installer
======================

<div align="center">
<a href="https://codefirst.iut.uca.fr/documentation/oki/book/oki/">Documentation</a> -
<a href="https://codefirst.iut.uca.fr/swagger?url=/git/oki/oki/raw/branch/main/oki-api.yaml">Repository API</a> -
<a href="https://oki-pkg.dev">Staging instance</a>
</div>
<br>

*oki(1)* is a package manager for C and C++ projects, focused on configuration simplicity.

*Oki* can download project's dependencies and provide an integration to the build process.

You can browse the staging instance at [oki-pkg.dev](https://oki-pkg.dev).

Specifications
--------------

*Oki* implements a dependency manager and a package repository.

The command line interface allows you to:

- bootstrap a new project using `oki init`
- search for packages to add with `oki list`
- install new dependencies with `oki install`
- make sure your dependencies are up-to-date using `oki update`
- publish your librairies to a package repository (`oki publish`)
- visualize your dependency graph (`oki tree`)

The package repository interface enables you to:

- look for newly published packages
- summarize package information
- access the package README
- view dependents packages

Conception (written in French)
------------------------------

- [CLI architecture](cli/ARCHITECTURE.md)
- [Repository architecture](web/ARCHITECTURE.md)
- [Conventions](CONVENTIONS.md)

Prerequisites
-------------

### Database
A PostgreSQL server is recommended in production. For development purposes, you may instead want to use an SQLite database.
On Debian: `apt install postgresql` / `apt install sqlite3`

### Web
The backend is written in PHP >= 7.4.
Depending on the database you have chosen, the appropriate PDO extensions are also required.
On Debian (versions may vary): `apt install php7.4-cli php7.4-pgsql php7.4-sqlite3`

### Command Line Interface
A C++ 20 compiler like GCC is needed, with the [nlohmann/json](https://github.com/nlohmann/json), [curl](https://github.com/curl/curl), [toml++](https://github.com/marzer/tomlplusplus), [boost::program_options](https://github.com/boostorg/program_options) and [minizip](https://github.com/madler/zlib/tree/master/contrib/minizip) libraries. Unit tests are using [doctest](https://github.com/doctest/doctest).
On Debian: `apt install build-essential nlohmann-json3-dev libcurl4-openssl-dev libminizip-dev libboost-program-options-dev doctest-dev`

Some commands also use some clang tools like [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) and [`gcovr`](https://github.com/gcovr/gcovr).
On Debian: `apt install clang-format gcovr`

Quick setup
-----------

```bash
cd cli
./configure.sh -d
make BUILD=release
sudo make install BUILD=release
```

Development
-----------

### Create a sample database and run the server
```bash
cd sql && ./create_sqlite_db.sh && cd ..
php -S localhost:8000 -t web/public
```

### Compile the cli
#### At home
```bash
cd cli && make
```
#### At the IUT
```bash
cd cli && ./make-in-vdn.sh && unset http_proxy
```

Inspiration
-----------

- [Cargo](https://doc.rust-lang.org/stable/cargo/), the Rust package manager for its manifest format and command organization
- [npm](https://www.npmjs.com), the JavaScript package manager for its lockfile handling and website presentation
- [pub](https://dart.dev/tools/pub/cmd), the Dart package manager for its dependency resolution algorithm
- [semver](https://semver.org), semantic versioning in order to know if a version is compatible with another
