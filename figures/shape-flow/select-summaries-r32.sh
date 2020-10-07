#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

source ${SCRIPT_PATH}/utils.sh
source ${PROJECT_PATH}/figures/utils.sh

IMAGE_ROOT_FOLDER=$1

OUTPUT_FOLDER=${SCRIPT_PATH}/output/shape-flow/summaries-r32
mkdir -p ${OUTPUT_FOLDER}

LEN_PEN="0.00098"
SHAPES="triangle square flower bean"

for SHAPE in ${SHAPES}
do
    cp $(get_summary_file ${IMAGE_ROOT_FOLDER} ${SHAPE} ${LEN_PEN}) ${OUTPUT_FOLDER}/${SHAPE}.eps
    convert_to_png ${OUTPUT_FOLDER}/${SHAPE}.eps ${OUTPUT_FOLDER}/${SHAPE}.png
    rm ${OUTPUT_FOLDER}/${SHAPE}.eps
done