#!/bin/bash

set -eu

VDN_SYSTEM=bigboss
FILES=oki

for i; do
  if [ $i == "coverage.html" -o $i == "oki-test" ]; then
    FILES="$FILES $i"
  fi
done

vdn-set-network-dir ~vdn/vdn/networks/demo
if ! vdn-alive $VDN_SYSTEM; then
  vdn-start -b $VDN_SYSTEM
  until vdn-ssh root@$VDN_SYSTEM : &> /dev/null; do
    sleep 2
  done
fi

currentDate=$(date +'%F %H:%M:%S')
vdn-ssh root@$VDN_SYSTEM "date -s '$currentDate' &> /dev/null && date -s '-2 hours'"

rsync -av src -e vdn-ssh root@$VDN_SYSTEM:
rsync -av test -e vdn-ssh root@$VDN_SYSTEM:
vdn-scp configure.sh root@$VDN_SYSTEM:
vdn-scp Makefile root@$VDN_SYSTEM:
vdn-ssh root@$VDN_SYSTEM << EOF
route del default gw 192.168.2.1
route add default gw 10.0.2.2
export http_proxy=http://193.49.118.36:8080/
if ! pkg-config nlohmann_json --exists || ! pkg-config libcurl --exists || ! pkg-config minizip --exists  || ! command -v gcovr &> /dev/null || ! [ -f /usr/include/doctest/doctest.h ]; then
  apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev libminizip-dev doctest-dev gcovr
fi
./configure.sh -d
make $*
EOF

for i in $FILES; do
  vdn-scp root@$VDN_SYSTEM:$i . 2> /dev/null
done