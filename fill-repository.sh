#!/bin/sh

set -eu

user="x"
pswd="x"

if [ -d packages ]; then
    cd packages && git pull --rebase --autostash && cd ..
else
    git clone https://codefirst.iut.uca.fr/git/oki/packages
fi

dest=$(dirname $(realpath "$0"))
for package in packages/*; do
    packageName=$(basename $package)
    echo "$package"
    cd $package && echo $user $pswd | oki publish 
    cd -
done
