#!/bin/bash

set -eu

VDN_SYSTEM=bigboss

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
vdn-scp Makefile root@$VDN_SYSTEM:
vdn-ssh root@$VDN_SYSTEM << EOF

route del default gw 192.168.2.1
route add default gw 10.0.2.2
export http_proxy=http://193.49.118.36:8080/
if ! pkg-config nlohmann_json --exists || ! pkg-config libcurl --exists; then
  apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev
fi
make
EOF
vdn-scp root@$VDN_SYSTEM:oki .

