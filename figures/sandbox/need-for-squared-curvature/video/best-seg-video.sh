#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
EXP_PATH="$( cd "${SCRIPT_PATH}" && cd .. && pwd )"
PROJECT_PATH="$( cd "$SCRIPT_PATH" && cd ../../.. && pwd)"


CACHE_TEMP_PATH="${PROJECT_PATH}/temp_cache"
mkdir -p ${CACHE_TEMP_PATH}
export MAGICK_TMPDIR=${CACHE_TEMP_PATH}

DATA_FOLDER="${EXP_PATH}/data"

OUTPUT_FOLDER=${SCRIPT_PATH}/output/video
mkdir -p ${OUTPUT_FOLDER}

function generate_video(){
    EXP_FOLDER=$1
    EXP_NAME=$2
    _OUTPUT_FOLDER=$3

    mkdir -p "${_OUTPUT_FOLDER}"

    ${SCRIPT_PATH}/make-plot-frames.sh "${EXP_FOLDER}/${EXP_NAME}" "${EXP_NAME}" ${CACHE_TEMP_PATH}
    ${SCRIPT_PATH}/join-gifs.sh "${EXP_FOLDER}/${EXP_NAME}" "${SCRIPT_PATH}/data/${EXP_NAME}" "${EXP_NAME}" "${_OUTPUT_FOLDER}/flow-plot.gif"
}


generate_video "${DATA_FOLDER}/perimeter-elastica" "elastica-g3-a1" "${OUTPUT_FOLDER}/elastica-g3-a1"
generate_video "${DATA_FOLDER}/perimeter-elastica" "perimeter-g3-a1" "${OUTPUT_FOLDER}/perimeter-g3-a1"

generate_video "${DATA_FOLDER}/elastica-flexibility-alpha1" "elastica-g8-a1" "${OUTPUT_FOLDER}/elastica-g8-a1"
generate_video "${DATA_FOLDER}/elastica-flexibility-alpha05" "elastica-g8-a0.5" "${OUTPUT_FOLDER}/elastica-g8-a0.5"


rm -rf ${CACHE_TEMP_PATH}
