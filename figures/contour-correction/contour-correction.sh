#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

source ${PROJECT_PATH}/figures/utils.sh

APP_GRAPH_SEG=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-seg-app

INPUT_FOLDER=${SCRIPT_PATH}/input
DATA_OUTPUT_FOLDER=${SCRIPT_PATH}/data
OUTPUT_FOLDER=${SCRIPT_PATH}/output/contour-correction

mkdir -p ${OUTPUT_FOLDER}

IMAGES="vase cat motorcycle"
for IMAGE in $IMAGES
do
    "${APP_GRAPH_SEG}" "${INPUT_FOLDER}/${IMAGE}.xml" -r7 -g1 -k2.5 -a0.01 -i2 -G5 -N0 "${DATA_OUTPUT_FOLDER}/${IMAGE}"

    mkdir -p ${OUTPUT_FOLDER}/${IMAGE}
    cp ${DATA_OUTPUT_FOLDER}/${IMAGE}/corrected-seg.png ${OUTPUT_FOLDER}/${IMAGE}/corrected-seg.png
    cp ${DATA_OUTPUT_FOLDER}/${IMAGE}/gc-seg.png ${OUTPUT_FOLDER}/${IMAGE}/gc-seg.png
done

