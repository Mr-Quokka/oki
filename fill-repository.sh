#!/bin/bash

set -eu

export OKI_USERNAME=test
export OKI_PASSWORD=test
export OKI_REPOSITORY=http://localhost:8000

while [ $# -ne 0 ] ; do
    case "$1" in
    "-r" | "--registry")
        OKI_REPOSITORY="$2"
        shift 2
        ;;
    *)
        echo "unknown option $1" >&2
        exit 1
        ;;
    esac
done

if [ -d packages ]; then
    cd packages && git pull --rebase --autostash && cd ..
else
    git clone https://codefirst.iut.uca.fr/git/oki/packages
fi

#L'ordre est important ! Publier un paquet qui dépend d'un paquet qui n'est pas encore publié empêche la publication.
for package in packages/{guess-mime-type,linked-list,mths,static-string-builder,shell-escape}; do
    packageName=$(basename $package)
    echo "Publishing $packageName"
    (cd $package && oki publish)
done
