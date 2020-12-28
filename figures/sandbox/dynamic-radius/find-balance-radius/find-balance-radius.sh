#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../.. && pwd)

FIND_BR_APP="${PROJECT_PATH}/cmake-build-release/figures/dynamic-radius/find-balance-radius/find-balance-radius"

# H="1.0 0.5 0.25 0.2 0.15 0.125 0.0625"
MAX_IT=100
RADIUS=10
OUTPUT_FOLDER="${SCRIPT_PATH}/output"

for h in $H
do
    ${FIND_BR_APP} $RADIUS $h $MAX_IT "${OUTPUT_FOLDER}/r${RADIUS}h${h}.txt"
done