#!/bin/sh

rm -f ../web/oki_packages.db

sed 's/SERIAL PRIMARY KEY/INTEGER PRIMARY KEY AUTOINCREMENT/g' tables.sql | sqlite3 ../web/oki_packages.db

sqlite3 ../web/oki_packages.db << EOF
INSERT INTO language VALUES (1, 'C');
INSERT INTO package VALUES (1, 'linked-list', 'My amazing linked list package', 1);
INSERT INTO version VALUES (1,1,'1.0.0-ALPHA',CURRENT_TIMESTAMP);
INSERT INTO package VALUES (3, 'linked-list2', 'My second linked list package', 1);
EOF

../fill-repository.sh