#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

source ${PROJECT_PATH}/figures/utils.sh

APP_GRAPH_FLOW=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app
APP_SUMMARY_FLOW=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/summary-flow

DATA_OUTPUT_FOLDER=${SCRIPT_PATH}/data
mkdir -p ${DATA_OUTPUT_FOLDER}

OUTPUT_FOLDER=${SCRIPT_PATH}/output/no-neighborhood-flow
mkdir -p ${OUTPUT_FOLDER}

SHAPES="triangle square flower"
for SHAPE in ${SHAPES}
do
    $APP_GRAPH_FLOW -S${SHAPE} -h0.125 -b1 -a0.015625 -r16 -v5 -O2 -N0 -n4 -i500 -B30 -d -s ${DATA_OUTPUT_FOLDER}/${SHAPE}
    $APP_SUMMARY_FLOW ${DATA_OUTPUT_FOLDER}/${SHAPE} ${DATA_OUTPUT_FOLDER}/${SHAPE}/summary.eps -j10 -e.png

    cp ${DATA_OUTPUT_FOLDER}/${SHAPE}/summary.eps ${OUTPUT_FOLDER}/${SHAPE}.eps
    convert_to_png ${OUTPUT_FOLDER}/${SHAPE}.eps ${OUTPUT_FOLDER}/${SHAPE}.png
    rm ${OUTPUT_FOLDER}/${SHAPE}.eps
done