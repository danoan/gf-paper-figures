#! /usr/bin/env bash

SCRIPT_PATH="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="${SCRIPT_PATH%gf-paper-figures*}gf-paper-figures"

INPUT_FOLDER="${SCRIPT_PATH}/input"
OUTPUT_FOLDER="${SCRIPT_PATH}/output"
mkdir -p "${OUTPUT_FOLDER}"

BUILD_PATH="${PROJECT_PATH}/build"
mkdir -p "${BUILD_PATH}"

cd "${BUILD_PATH}"
cmake -DCMAKE_PREFIX_PATH=/home/daniel/LocalLibs -DBOOST_INCLUDE_DIRS=/home/daniel/LocalLibs/include -DBOOST_LIBS_DIR=/home/daniel/LocalLibs/lib -DCMAKE_INSTALL_PREFIX=/home/daniel/Projects/Git/phd-stuff/gf-paper-figures/install -DCMAKE_BUILD_TYPE=release -DUSE_REMOTE_REPOSITORIES=ON -DBUILD_TESTS=ON ..

make install 
 
