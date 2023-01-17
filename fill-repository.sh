#!/bin/bash

set -eu

export OKI_USERNAME=xxxx
export OKI_PASSWORD=x
export OKI_REPOSITORY=https://oki-pkg.dev

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

dest=$(dirname $(realpath "$0"))
#L'ordre est important ! Publier un paquet qui dépend d'un paquet qui pas encore publié empêche la publication.
for package in packages/{guess-mime-type,linked-list,mths,static-string-builder,shell-escape} ; do
    packageName=$(basename $package)
    echo "Publication du paquet $package"
    (cd $package && oki publish)
done
