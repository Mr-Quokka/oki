---
title: oki
section: 1
header: User Manual
---

# NAME

oki - A C and C++ package manager

# SYNOPSIS

`oki <command> [<args>]`

# DESCRIPTION

This program is a package manager focused on configuration simplicity.

# COMMANDS

## Build Commands

**oki-makefile**(1)
: Create or update a Makefile.

## Manifest Commands

**oki-tree**(1)
: Display a tree visualization of the dependency graph.

## Package Commands

**oki-init**(1)
: Create a new package in an existing directory.

**oki-install**(1)
: Install new dependencies.

**oki-list**(1)
: Search packages.

**oki-uninstall**(1)
: Remove a dependency.

## Publishing Commands

**oki-package**(1)
: Assemble the local package into a distributable tarball.

**oki-publish**(1)
: Upload a package to the repository.

# EXAMPLES

1. Create a new oki project in the current directory:

       oki init c

2. Generate a Makefile for the project:

       oki makefile

3. Install a dependency:

       oki install string-builder

4. Create a distribuable tarball:

       oki package

5. Learn about a command's options and usage:

       oki help tree

# BUGS

See <https://codefirst.iut.uca.fr/git/oki/oki> for issues.
