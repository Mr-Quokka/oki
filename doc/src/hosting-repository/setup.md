# Set up a repository

Packages are published in repositories.
By default, oki uses the default one, [oki-pkg.dev](https://oki-pkg.dev), but you may want to set up your own repository to host your private packages.

Oki provides a PHP implementation of a repository following the Oki API specification.
It can also be used for your own server.

This guide explains how to set up it.

## Prerequisites

The Oki repository implementation is written in PHP 7.4 but using the [latest version of PHP](https://www.php.net/supported-versions.php) is highly recommended.

The backend use a PostgreSQL database, and you will also need [Composer](https://getcomposer.org) to install some dependencies used for specific features.

On Debian:

```bash
apt install postgresql php8.2-fpm php8.2-pgsql php8.2-zip composer
```

## Installation

Create a user and a database on your PostgreSQL server.

```console
$ sudo -i -u postgres psql
postgres=# CREATE DATABASE oki;
postgres=# CREATE USER oki WITH ENCRYPTED 'password';
postgres=# GRANT ALL PRIVILEGES ON DATABASE oki TO oki;
```

Clone the repository

```bash
# Specify the path where the server will be installed
dest=/var/www/oki
mkdir -p $dest

# Clone the repository and move the server implementation
git clone --depth 1 https://codefirst.iut.uca.fr/git/oki/oki /tmp/oki
mv /tmp/oki/web/. $dest

# Create the directory where packages will be uploaded
mkdir -p $dest/public/packages
chown -R www-data:www-data $dest/public/packages

# Setup the database
psql -h localhost -d oki -U oki < /tmp/oki/sql/tables.sql

# Update the configuration to connect to the database
sed -i 's/new SQLiteConfig/new \\Oki\\Config\\PostgreSQLConfig/' \
  $dest/src/Oki/Config/Config.php
vi $dest/src/Oki/Config/Config.php

# Remove cloned files
rm -r /tmp/oki
```

## Nginx configuration

Nginx needs to serve the `public` directory and redirect every request to a non-existing file to the `public/index.php`.

```nginx
server {
    # Listen and SSL configuration...

    # The directory where oki is installed
    root /var/www/oki/public;

    # Use index.php as the default requested path
    index index.php;

    # Your server name
    server_name oki-pkg.dev www.oki-pkg.dev;
    
    location / {
        # Try to serve the URI as a file, then as a directory, then redirect it to PHP
        try_files $uri $uri/ /index.php?$query_string;

        location ~ \.php$ {
            include snippets/fastcgi-php.conf;
            fastcgi_pass unix:/var/run/php/php8.2-fpm.sock;
        }
    }
}
```

### Subdirectory

You will need to adjust the `BASE_PATH` config in `src/Config/Config.php`.

```nginx
    location ^~ /oki/ {
        alias /var/www/oki/public/;
        try_files $uri $uri/ /oki/index.php;

        location ~ \.php$ {
            include snippets/fastcgi-php.conf;
            fastcgi_param SCRIPT_FILENAME $request_filename;
            fastcgi_pass unix:/var/run/php/php8.2-fpm.sock;
        }
    }
```

## Apache configuration

```apache
<VirtualHost *:443>
    ServerName oki-pkg.dev
    ServerAlias www.oki-pkg.dev
    DocumentRoot /var/www/oki/public

    # Listen and SSL configuration...

	<Directory /var/www/oki/public>  
		Options All
		AllowOverride All
		Order Allow,Deny
		Allow from all
        FallbackResource /public/index.php
	</Directory>
</VirtualHost>
```
