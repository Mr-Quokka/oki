# First Steps

> Some commands used here are not fully implemented and may change in the near future.

## Init

To create a new package with Oki, use `oki init` in the project root directory:

```console
$ mkdir hello-world && cd hello-world
$ oki init c
Created `hello-world` package
```

Oki will generate an `oki.toml` file. This manifest describes the package and its dependencies:

```toml
[package]
name = "hello-world"
version = "0.1.0"
kind = "c"

[dependencies]
```

Oki generated a default `main.c` file:

```c
#include <stdio.h>

int main(void) {
    printf("Hello, world!\n");
    return 0;
}
```

In order to compile it, Oki can generate a Makefile with the `oki makefile` command:

```console
$ ls
oki.toml  src
$ oki makefile
Generated Makefile
$ ls
Makefile  oki-lock.toml  oki.toml  src
```

`make` can now be used to compile:

```console
$ make
mkdir build
gcc -std=c17 -Wall -Wextra -pedantic -g -MMD -MP -c src/main.c -o build/main.o
gcc -o hello-world build/main.o
$ ./hello-world
Hello, world!
```

## Add a dependency

Dependencies are listed in the `oki.toml` file. This file can be edited to add a new dependency, but you can also use the `oki install` command: 

For instance, to add the latest version of the `mths` package:

```console
$ oki install mths
Adding mths v0.1.0 to dependencies.

 + mths 0.1.0
Installed 1 package
```

Finally, regenerate the Makefile to use this newly added dependency:

```console
$ oki makefile
Generated Makefile
```
