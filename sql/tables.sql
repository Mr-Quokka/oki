CREATE TABLE langage (
	id SERIAL PRIMARY KEY,
	designation VARCHAR(30)
);

CREATE TABLE package (
	id SERIAL PRIMARY KEY,
	short_name VARCHAR(30) NOT NULL UNIQUE,
	long_name VARCHAR(50) NOT NULL,
	langage_id INT NOT NULL REFERENCES langage(id)
);

CREATE TABLE version (
	id SERIAL PRIMARY KEY,
	package_id INT NOT NULL REFERENCES package(id),
	identifier VARCHAR(10) NOT NULL,
	published_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	UNIQUE (package_id, identifier)
);
