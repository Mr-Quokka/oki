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

Quick setup
-----------

### Create a sample database and run the server
```bash
cd sql && ./create_sqlite_db.sh && cd ..
php -S localhost:8000 -t web/public
```

### Compile the cli
#### At home
```bash
cd cli 
apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev 
make
```
#### At the IUT
```bash
cd cli && ./make-in-vdn.sh && unset http_proxy
```
