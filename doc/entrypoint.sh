#!/bin/bash

set -eu

rsync --rsync-path="mkdir -p /usr/share/nginx/html/$DRONE_REPO_OWNER/book/$DRONE_REPO_NAME/ && rsync" -e "ssh -o StrictHostKeyChecking=no" -avz --delete doc/book/ root@nginx:/usr/share/nginx/html/$DRONE_REPO_OWNER/book/$DRONE_REPO_NAME/
