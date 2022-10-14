#!/bin/sh

set -eu

if [ -d packages ]; then
    cd packages && git pull --rebase --autostash && cd ..
else
    git clone https://codefirst.iut.uca.fr/git/oki/packages
fi

dest=$(dirname $(realpath "$0"))
requests=''
for package in packages/*; do
    packageName=$(basename $package)
    existingPackage="$dest"/web/public/packages/${packageName}_0.1.zip
    rm -f "$existingPackage"
    cd $package/src && zip -r "$existingPackage" ./*
    cd -
    requests="$requests
    INSERT INTO package (short_name, long_name, langage_id) VALUES ('$packageName', 'A random package', (SELECT id_language FROM language WHERE designation = 'C'));
    INSERT INTO version (package_id, identifier, published_date) VALUES ((SELECT id_package FROM package WHERE short_name='$packageName'), '0.1', CURRENT_TIMESTAMP);"
done

echo "$requests" | sqlite3 "$dest"/web/oki_packages.db
