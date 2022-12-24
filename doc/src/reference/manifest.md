# The manifest Format

The `oki.toml` file at the root directory of each package is called its *manifest*.
It is written in the [TOML](https://toml.io) format and contains metadata that is needed to use the package.

## The `[package]` section

### The `name` field

The package name is an unique identifier.

The name must use only alphanumeric characters or `-`, and cannot be empty.
Most repositories also impose a maximum of 64 characters.

### The `version` field

Every package must follow the [semantic versioning specification](https://semver.org).

This convention allow to determine what is compatible between different versions of a package.

### The `description` field

The description provides a brief overview about the package.
