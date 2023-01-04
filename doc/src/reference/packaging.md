# Packaging a `.deb` for OKI

Go to `cli` directory and execute `make dist`.

Copy the `.tar.gz` archive which was created in the directory where you want to create the `.deb` file.

Rename the archive by changing the hyphen with an underscore and ad `.orig` before the `.tar.gz`. For example:

```bash
mv oki-0.1.0.tar.gz oki_0.1.0.orig.tar.gz
```

Extract the archive:

```bash
tar xf oki_0.1.0.orig.tar.gz && cd oki-0.1.0
```

Create a templated `debian` folder with `dh_make`. Oki is a single package (`s`):
```bash
dh_make --native
```

Go to the `debian` folder.
The `configure.sh` script should be executed before building and Oki should be installed in `/usr`.

Edit the `debian/rule` file like this:

```Makefile
#!/usr/bin/make

%:
	dh $@

build:
	sh ./configure.sh -d

override_dh_auto_install:
	dh_auto_install -- prefix=/usr BUILD=release
```

Likewise for the `debian/control` file:

```
Source: oki
Section: devel
Priority: optional
Maintainer: Lou VALADE <lovalade@etu.uca.fr>
Build-Depends: debhelper-compat (= 12), nlohmann-json3-dev, libcurl4-openssl-dev, libminizip-dev, doctest-dev
Standards-Version: 4.5.1
Homepage: https://oki-pkg.dev
Vcs-Browser: https://codefirst.iut.uca.fr/git/oki/oki
Vcs-Git: https://codefirst.iut.uca.fr/git/oki/oki.git
Rules-Requires-Root: no

Package: oki
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}
Description: A package manager for C and C++ applications
 Oki is a package manager that allow you to publish, install, update, manage and include packages
```

Create a non-signed `.deb` package with `debuild -us -uc`.

You can now install it with `dpkg -i ../oki_[version]_[architecture].deb`.
