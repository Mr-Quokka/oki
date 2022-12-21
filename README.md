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
A C++ 20 compiler like GCC is needed, with the [nlohmann/json](https://github.com/nlohmann/json), [curl](https://github.com/curl/curl) and [minizip](https://github.com/madler/zlib/tree/master/contrib/minizip) libraries. Unit tests are using [doctest](https://github.com/doctest/doctest).
On Debian: `apt install build-essential nlohmann-json3-dev libcurl4-openssl-dev libminizip-dev doctest-dev`

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
