#!/bin/sh

sqlite3 ../web/oki_packages.db < tables.sql

sqlite3 ../web/oki_packages.db << EOF
INSERT INTO language VALUES (1, 'C');
INSERT INTO package VALUES (1, 'linked-list', 'My amazing linked list package', 1);
EOF
