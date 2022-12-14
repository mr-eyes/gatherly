#!/usr/bin/env bash

# exit when any command fails
set -o nounset
set -o errexit
set -x
set -euox pipefail
set -e

# force clean it up
function cleanup() {
    echo "REMOVING OLD FILES IF EXISTS";
    rm -rf build/temp*
    rm -rf build/lib.linux*
    rm -rf dist/*
    rm -rf __pycache__/
    rm -rf rm -rf gatherly.egg-info/
    rm -rf build/bdist.linux-x86_64
    rm -rf dist
}

trap cleanup EXIT
cleanup

# Build the project if not already built
BUILD_DIR="build"
[[ -d ${BUILD_DIR} ]] || cmake -Bbuild && cmake --build build -j4


echo "BDIST WHEEL"
$(which python) setup.py bdist_wheel

cd dist/

$(which python) -m pip uninstall gatherly -y

$(which python) -m pip install gatherly*cp*.whl

rm -rf build/temp*
rm -rf build/lib.linux*
rm -rf dist/*
rm -rf __pycache__/
rm -rf gatherly.egg-info/
rm -rf build/bdist.linux-x86_64