#!/bin/bash

set -eu

vdn-set-network-dir ~vdn/vdn/networks/demo
if ! vdn-alive bigboss; then
  vdn-start -b bigboss
fi
vdn-scp -r src root@bigboss:
vdn-scp -r Makefile root@bigboss:
vdn-ssh root@bigboss << EOF
route del default gw 192.168.2.1
route add default gw 10.0.2.2
export http_proxy=http://193.49.118.36:8080/
apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev
make
EOF
vdn-scp root@bigboss:oki .

