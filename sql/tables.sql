CREATE TABLE language (
    id_language SERIAL PRIMARY KEY,
    designation VARCHAR(30)
);

CREATE TABLE package (
    id_package SERIAL PRIMARY KEY,
    name VARCHAR(64) NOT NULL UNIQUE,
    description VARCHAR(256) NOT NULL,
    language_id INT NOT NULL REFERENCES language(id_language)
);

CREATE TABLE version (
    id_version SERIAL PRIMARY KEY,
    package_id INT NOT NULL REFERENCES package(id_package),
    identifier VARCHAR(64) NOT NULL,
    published_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (package_id, identifier)
);

CREATE TABLE dependency (
    package_reference_id INT REFERENCES package(id_package),
    constrainer_id INT NOT NULL REFERENCES version(id_version),
    constraint_value VARCHAR(64) NOT NULL,
    PRIMARY KEY (package_reference_id, constrainer_id)
);

CREATE TABLE registered_user (
    id_user SERIAL PRIMARY KEY,
    login VARCHAR(64) NOT NULL,
    password VARCHAR(72) NOT NULL,
    permissions INT NOT NULL,
    UNIQUE (login)
);


CREATE TABLE ownership (
    user_id INT REFERENCES registered_user(id_user),
    package_id INT REFERENCES package(id_package),
    is_pending BOOLEAN DEFAULT false NOT NULL,
    PRIMARY KEY (user_id, package_id)
);
