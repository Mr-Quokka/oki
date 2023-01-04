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

### The `kind` field

Any project should specify its main type. Types are usually language related:

- `c`: a C project
- `cpp`: a C++ project

### The `include` field

Oki only always includes the `oki.toml` file when publishing a package.

The `include` field can be used to specify which files are included.
It should contain every source directory needed to compile and not any compilation result.

## The `[lib.c]` section

For a library, a package used by other packages, this section describes how to plug it to a C-like compiler.

### The `export` field

The `export` field defines in which directory header files are located in.

### The `build-static` field

A command to automatically statically build the package.
It usually invokes `make` or `cmake` depending on the build system.
Note that Oki itself is not a build system, each library should provide a way to be built, which be called by Oki.

### The `static-link` field

The file to statically link.
It should be the result of the `build-static` command.
