# How to make a `.deb` for OKI

Go to `cli` directory and execute `make dist`.

Copy the`.tar.gz` archive wich was created in the directory where you want to create the `.deb` file.

Rename the archive by changing the hyphen with an underscore and ad `.orig` before the `.tar.gz`.

Extract the archive.

For example : `oki-1.0.tar.gz` -> `oki_1.0.orig.tar.gz`

Go into the directory extract.

Execute `dh_make --native`.

Press `s` to say single.

And `y` to accept the display informations.

Go to the `debian` folder.

Edit the `rule` file like this :

```
#!/usr/bin/make

%:
    dh $@

build:
    sh./configure.sh -d

override_dh_auto_install:
    dh_auto_install -- prefix=/usr BUILD=release
```

Likewise for the `control` file :

```
Source: oki
Section: devel
Priority: optional
Maintainer: Lou VALADE <lovalade@etu.uca.fr>
Build-Depends: debhelper-compat (= 12), nlohmann-json3-dev, libcurl4-openssl-dev, libminizip-dev, doctest-dev
Standards-Version: 4.5.1
Homepage: oki-pkg.dev
Vcs-Browser: https://codefirst.iut.uca.fr/git/oki/oki.git
Vcs-Git: https://codefirst.iut.uca.fr/git/oki/oki.git
Rules-Requires-Root: no

Package: oki
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}
Description: allow to install and manage packages
 oki is a package manager that allow you to publish, install, update, manage and include packages
```
Execute `debuild -us -uc`

Execute `dpkg -i ../oki_[version]_amd64.deb`