#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

source ${PROJECT_PATH}/figures/utils.sh

APP_GRAPH_SEG=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-seg-app

INPUT_FOLDER=${SCRIPT_PATH}/input
DATA_OUTPUT_FOLDER=${SCRIPT_PATH}/data
OUTPUT_FOLDER=${SCRIPT_PATH}/output/completion

function create_output(){
    DATA_OUTPUT_FOLDER=$1
    OUTPUT_FOLDER=$2
    IMAGE=$3

    mkdir -p ${OUTPUT_FOLDER}/${IMAGE}
    cp ${DATA_OUTPUT_FOLDER}/${IMAGE}/corrected-seg.png ${OUTPUT_FOLDER}/${IMAGE}/corrected-seg.png
    cp ${DATA_OUTPUT_FOLDER}/${IMAGE}/gc-seg.png ${OUTPUT_FOLDER}/${IMAGE}/gc-seg.png
}

mkdir -p ${OUTPUT_FOLDER}

#"${APP_GRAPH_SEG}" "${INPUT_FOLDER}/brown-snake.xml" -i50 -r7 -g0.001 -k10 -a0 -G5 -N30 -O2 -n32 -w -d -Hrandom "${DATA_OUTPUT_FOLDER}/brown-snake"
#create_output ${DATA_OUTPUT_FOLDER} ${OUTPUT_FOLDER} "brown-snake"
cp ${INPUT_FOLDER}/brown-snake.jpg ${OUTPUT_FOLDER}/brown-snake.jpg

#"${APP_GRAPH_SEG}" "${INPUT_FOLDER}/pear.xml" -i-1 -r7 -g0.002 -k10 -a0.001 -G5 -N30 -n32 -w -Hrandom "${DATA_OUTPUT_FOLDER}/pear"
#create_output ${DATA_OUTPUT_FOLDER} ${OUTPUT_FOLDER} "pear"
cp ${INPUT_FOLDER}/pear.jpg ${OUTPUT_FOLDER}/pear.jpg
