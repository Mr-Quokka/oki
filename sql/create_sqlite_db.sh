#!/bin/sh

rm -f ../web/oki_packages.db

sed 's/SERIAL PRIMARY KEY/INTEGER PRIMARY KEY AUTOINCREMENT/g' tables.sql | sed '/^CREATE INDEX/d' | sqlite3 ../web/oki_packages.db

sqlite3 ../web/oki_packages.db << EOF
INSERT INTO language VALUES (1, 'c'), (2, 'cpp');
EOF

php ../web/fixtures/InitialUser.php
php -q -S localhost:8000 -t ../web/public > /dev/null &
trap 'kill $!' EXIT
sleep 1
../fill-repository.sh
