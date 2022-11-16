CREATE TABLE language (
	id_language SERIAL PRIMARY KEY,
	designation VARCHAR(30)
);

CREATE TABLE package (
	id_package SERIAL PRIMARY KEY,
	short_name VARCHAR(30) NOT NULL UNIQUE,
	description VARCHAR(50) NOT NULL,
	language_id INT NOT NULL REFERENCES language(id_language)
);

CREATE TABLE version (
	id_version SERIAL PRIMARY KEY,
	package_id INT NOT NULL REFERENCES package(id_package),
	identifier VARCHAR(10) NOT NULL,
	published_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	UNIQUE (package_id, identifier)
);

CREATE TABLE user (
	id_user SERIAL PRIMARY KEY,
	login VARCHAR(30) NOT NULL UNIQUE,
	password VARCHAR(72),
	perm INT NOT NULL
);

CREATE TABLE authorization (
	role VARCHAR(30) PRIMARY KEY
);