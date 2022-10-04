CREATE TABLE language (
	id_language SERIAL PRIMARY KEY,
	designation VARCHAR(30)
);

CREATE TABLE package (
	id_package SERIAL PRIMARY KEY,
	short_name VARCHAR(30) NOT NULL UNIQUE,
	long_name VARCHAR(50) NOT NULL,
	langage_id INT NOT NULL REFERENCES langage(id_language)
);

CREATE TABLE version (
	id_version SERIAL PRIMARY KEY,
	package_id INT NOT NULL REFERENCES package(id_package),
	identifier VARCHAR(10) NOT NULL,
	published_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	UNIQUE (package_id, identifier)
);
