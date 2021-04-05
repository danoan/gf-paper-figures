#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$(dirname "${BASH_SOURCE[0]}")" > /dev/null 2>&1 && pwd )"
PROJECT_PATH="$( cd "${SCRIPT_PATH}" && cd ../.. && pwd)"

NFSC_APP="${PROJECT_PATH}/cmake-build-release/figures/need-for-squared-curvature/need-for-squared-curvature"
BEST_SEG_APP="${PROJECT_PATH}/cmake-build-release/figures/need-for-squared-curvature/best-segmentation"

GRID_STEP=0.03125
RADIUS=0.25


function linearIncrement(){
    INITIAL=$1
    FINAL=$2
    INC=$3
    S=""

    KG="True"
    while [ $KG = "True" ]
    do
        S="${S} ${INITIAL}"
        INITIAL=$(python -c "print(${INITIAL}+${INC})")

        KG=$(python -c "print( ${INITIAL}<${FINAL} if ${INC} > 0 else ${INITIAL}>${FINAL})")
    done
    S="${S} ${FINAL}"
    echo ${S}
}

function compute_energy(){
    ENERGY=$1;shift
    ALPHA=$1;shift
    GAP=$1;shift
    WIDTH=$1;shift
    __OUTPUT_FOLDER=$1;shift
    FIRST_HEIGHT=$1;shift
    HEIGHT=${FIRST_HEIGHT}

    IT=0

    OUT=""
    printf -v OUT "#WIDTH HEIGHT connected-${ENERGY} disconnected-${ENERGY} ALPHA=${ALPHA} GAP=${GAP}\n"
    while [ -n "${HEIGHT}" ]
    do
        CONN_EN_VALUE=$( ${NFSC_APP} ${WIDTH} ${HEIGHT} ${RADIUS} ${GAP} ${ALPHA} ${GRID_STEP} "connected-${ENERGY}" )
        DISCONN_EN_VALUE=$( ${NFSC_APP} ${WIDTH} ${HEIGHT} ${RADIUS} ${GAP} ${ALPHA} ${GRID_STEP} "disconnected-${ENERGY}" )

        ${BEST_SEG_APP} ${WIDTH} ${HEIGHT} ${RADIUS} ${GAP} ${ALPHA} ${GRID_STEP} ${ENERGY} "${__OUTPUT_FOLDER}/$( printf "%04d" ${IT} ).png" 467

        printf -v OUT "${OUT}${WIDTH} ${HEIGHT} ${CONN_EN_VALUE} ${DISCONN_EN_VALUE}\n"

        HEIGHT=$1;shift
        IT=$(( $IT+1 ))
    done
    echo "${OUT}" > "${__OUTPUT_FOLDER}/energy.txt"
}

function generate_data(){
    ALPHA=$1
    GAP=$2
    INITIAL_HEIGHT=$3
    FINAL_HEIGHT=$4
    WIDTH=$5
    _OUTPUT_FOLDER=$6

    mkdir -p ${_OUTPUT_FOLDER}

    HEIGHT_LIST=$(linearIncrement ${INITIAL_HEIGHT} ${FINAL_HEIGHT} -0.5)

    compute_energy "perimeter" ${ALPHA} ${GAP} ${WIDTH} "${_OUTPUT_FOLDER}/perimeter-g${GAP}-a${ALPHA}" ${HEIGHT_LIST}
    compute_energy "elastica" ${ALPHA} ${GAP} ${WIDTH} "${_OUTPUT_FOLDER}/elastica-g${GAP}-a${ALPHA}" ${HEIGHT_LIST}
}


OUTPUT_FOLDER="${SCRIPT_PATH}/data"

generate_data 1 3 10 1 10 ${OUTPUT_FOLDER}/perimeter-elastica
generate_data 1 8 10 1 10 ${OUTPUT_FOLDER}/elastica-flexibility-alpha1
generate_data 0.5 8 10 1 10 ${OUTPUT_FOLDER}/elastica-flexibility-alpha05