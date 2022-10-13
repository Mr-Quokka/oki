#!/bin/sh

temp=$(mktemp -d)
mkdir "$temp"/mths
echo '#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)' > "$temp"/mths/mths.c
dest=$(dirname $(realpath "$0"))
echo "$dest"/web/public/packages/mths_0.1.zip
cd "$temp"/mths && zip -r "$dest"/web/public/packages/mths_0.1.zip ./*
rm -r "$temp"
