#!/bin/sh

rm -f ../web/oki_packages.db

sed 's/SERIAL PRIMARY KEY/INTEGER PRIMARY KEY AUTOINCREMENT/g' tables.sql | sqlite3 ../web/oki_packages.db

sqlite3 ../web/oki_packages.db << EOF
INSERT INTO language VALUES (1, 'C');
INSERT INTO package VALUES (1, 'linked-list', 'My amazing linked list package', 1);
INSERT INTO version VALUES (1,1,'1.0.0-ALPHA',CURRENT_TIMESTAMP);
INSERT INTO package VALUES (2, 'mths', 'Min and max library', 1);
INSERT INTO version VALUES (2, 2, '0.1', CURRENT_TIMESTAMP);
EOF

if [ ! -f ../web/public/packages/linked-list_1.0.0-ALPHA.zip ] ; then
    mkdir -p ../web/public/packages
    touch ../web/public/packages/linked-list_1.0.0-ALPHA.zip
fi
