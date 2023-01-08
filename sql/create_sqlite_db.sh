#!/bin/sh

rm -f ../web/oki_packages.db

sed 's/SERIAL PRIMARY KEY/INTEGER PRIMARY KEY AUTOINCREMENT/g' tables.sql | sqlite3 ../web/oki_packages.db

sqlite3 ../web/oki_packages.db << EOF
INSERT INTO language VALUES (1, 'c'), (2, 'cpp');
INSERT INTO package VALUES (3, 'linked-list2', 'My second linked list package', 1);
INSERT INTO dependency VALUES (1,1,"<8.2.1");
EOF

../fill-repository.sh
