# Dependencies

## Declaring dependencies

Dependencies are listed in the `oki.toml` file.
This file can be edited to add a new dependency by specifying the package name and the version you want to use.

For instance, to add the `mths` package in version `0.1.0` or later:

```toml
[package]
name = "hello-world"
version = "0.1.0"
kind = "c"

[dependencies]
mths = "0.1.0"
```

Then fetch this dependency with `oki fetch`:

```console
$ oki fetch
 + mths 0.1.0
Installed 1 package
```

> `oki fetch` will always try to download up-to-date dependencies based on the manifest and the manifest lock.

If you have retrieved a project from a Git repository for example, it should not contain any dependency.
Instead, you will be required to fetch all the dependencies declared in the manifest by using `oki fetch`.

These two steps can be resumed with the `oki install` command:

```console
$ oki install mths
Adding mths v0.1.0 to dependencies.

 + mths 0.1.0
Installed 1 package
```

## SemVer

Oki uses the [semantic versioning specification](https://semver.org) to determine is compatible between different versions of a package.

Dependencies specify a compatible version range they want to rely on:

|Requirement|Example|Equivalence|Description|
|--|--------|--|-------------|
|Caret|`1.2.3` or `^1.2.3`| `>=1.2.3,<2.0.0`|Any greater or equal version without a major version bump.|
|Tilde|`~1.2`|`>=1.2.0,<1.3.0`|Any greater or equal version without a major or minor version bump.|
|Equals| `=1.2.3`|`=1.2.3`|Exactly the specified version only.|
|Wildcard|`*` |`Ω`|Any version.|
