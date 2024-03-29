#!/bin/bash

set -eu

VDN_SYSTEM=bigboss
FILES=oki

for target; do
  if [ "$target" == 'coverage.html' ] || [ "$target" == 'oki-test' ]; then
    FILES="$FILES $target"
  fi
done

vdn-set-network-dir ~vdn/vdn/networks/demo
if ! vdn-alive $VDN_SYSTEM; then
  vdn-start -b $VDN_SYSTEM
  until vdn-ssh root@$VDN_SYSTEM : &> /dev/null; do
    sleep 2
  done
fi

currentDate=$(date -u +'%F %H:%M:%S')
vdn-ssh root@$VDN_SYSTEM "date -s '$currentDate' &> /dev/null"

rsync -av src test man contrib -e vdn-ssh root@$VDN_SYSTEM:
vdn-scp configure.sh root@$VDN_SYSTEM:
vdn-scp Makefile root@$VDN_SYSTEM:
vdn-ssh root@$VDN_SYSTEM << EOF
route del default gw 192.168.2.1
route add default gw 10.0.2.2
export http_proxy=http://proxycl.iut.uca.fr:8080
export https_proxy=$http_proxy
if ! pkg-config nlohmann_json --exists || ! pkg-config libcurl --exists || ! pkg-config minizip --exists || ! command -v gcovr &> /dev/null || ! [ -f /usr/include/doctest/doctest.h ] || ! [ -d /usr/share/doc/libboost-program-options-dev ]; then
  apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev libminizip-dev libboost-program-options-dev doctest-dev gcovr
fi
./configure.sh -d
make $*
EOF

for file in $FILES; do
  vdn-scp root@$VDN_SYSTEM:"$file" . 2> /dev/null
done
