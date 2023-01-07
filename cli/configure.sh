#!/bin/sh

canDownload=false
if [ "$1" = '-d' ]; then
    canDownload=true
fi

set -eu

exitIfCantDownload() {
    if [ "$canDownload" = false ]; then
        echo "$1 is not detected. Use ./configure.sh -d to download it locally."
        exit 1
    fi
}

detectNlohmannJson() {
    if pkg-config nlohmann_json --exists; then
	    echo 'Using existing nlohmann/json installation.'
	    return
    fi

    if [ -d third-party/nlohmann/json.hpp ]; then
	    return
    fi
    mkdir -p third-party/nlohmann
    exitIfCantDownload 'nlohmann/json'
    echo 'Downloading nlohmann/json from GitHub.' 
    wget 'https://raw.githubusercontent.com/nlohmann/json/v3.11.2/single_include/nlohmann/json.hpp' -O third-party/nlohmann/json.hpp
}

detectTomlPlusPlus() {
    if [ -f third-party/toml.hpp ]; then
	    return
    fi
    mkdir -p third-party
    exitIfCantDownload 'marzer/tomlplusplus'
    echo 'Downloading marzer/tomlplusplus from GitHub.' 
    wget 'https://raw.githubusercontent.com/marzer/tomlplusplus/master/toml.hpp' -O third-party/toml.hpp
}

detectDoctest() {
    if [ -f /usr/include/doctest/doctest.h ]; then
        echo 'Using existing doctest/doctest installation.'
        return
    fi

    if [ -f third-party/doctest/doctest.hpp ]; then
	    return
    fi
    mkdir -p third-party/doctest
    exitIfCantDownload 'doctest/doctest'
    echo 'Downloading doctest/doctest from GitHub.'
    wget 'https://raw.githubusercontent.com/doctest/doctest/v2.4.9/doctest/doctest.h' -O third-party/doctest/doctest.hpp
}

detectNlohmannJson
detectTomlPlusPlus
detectDoctest

for lib in libcurl minizip; do
    pkg-config --exists --print-errors "$lib"
done

echo 'Found all necessary libraries.'
