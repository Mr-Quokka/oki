# Repositories

A repository centralize a collection of packages that are available for installation by users.
Oki supports two types of repositories, local and remote, and can use several at the same time.

Repositories can be configured in the `sources` section of the `~/.config/oki.toml` file.
Every repository is identified by a name, and a URL or filesystem path.

Two repository names should have a special meaning:

- `default`: the repository to publish packages to, by default set to [oki-pkg.dev](https://oki-pkg.dev).
- `local`: the local repository, by default located at `~/.local/share/oki`.

## Local repository

A local repository is a package repository that is stored on the user's local computer in the `~/.local/share/oki` directory, rather than on a remote server.

It is often used to test packages before publishing them to a real repository, or to store private packages that are used only by a single user or project.

The local repository is by default the first repository checked when searching a package to install.

## Remote repository

A remote repository is a package repository that is stored on a remote server, and can be accessed by users over the network.

The main and default repository is [oki-pkg.dev](https://oki-pkg.dev), but you can add new ones in the `~/.config/oki.toml` file.

For instance, to add an `oki.example.com` repository, you can add the following lines:

```toml
[sources]
example = "https://oki.example.com"
```

The `oki.example.com` repository will now be the first repository checked when searching a package to install.

If you want `oki-pkg.dev` to be the first repository checked, then explicitly add it to the `sources` list before the `oki.example.com` repository:

```toml
[sources]
default = "https://oki-pkg.dev"
example = "https://oki.example.com"
```

## Repository priority

When searching for a package to install, Oki will check the repositories in the order they are listed in the `sources` section of the `~/.config/oki.toml` file.

If a package is found in multiple repositories, only the one with the highest priority will be used.

If you want to use a package from a repository with a lower priority, you can use the `--registry` option in most commands to specify the repository to use:

```bash
oki install --registry example my-package
```
