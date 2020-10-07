#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

LEGNTH_RATIO_APP="${PROJECT_PATH}/cmake-build-release/figures/length-ratio-decrease/length-ratio-decrease"
GENERATE_DISK_APP="${PROJECT_PATH}/cmake-build-release/figures/limit-radius/generate-disk"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app"

DISK_RADIUS=$1
EVOLUTION_RADIUS=$2
ESTIMATION_RADIUS=$3

OUTPUT_FOLDER="${SCRIPT_PATH}/output"
mkdir -p ${OUTPUT_FOLDER}

DISK_IMAGE="${OUTPUT_FOLDER}/disk.png"
${GENERATE_DISK_APP} "${DISK_RADIUS}" "${DISK_IMAGE}"
${GRAPH_FLOW_APP} -S"${DISK_IMAGE}" -h1.0 -a0 -b1 -r"${EVOLUTION_RADIUS}" -N0 -O2 -i15 -s ${OUTPUT_FOLDER}/temp

P="0"
TK2="0"
for F in $(ls ${OUTPUT_FOLDER}/temp/*.png)
do
    OUT=$( ${LEGNTH_RATIO_APP} "${F}" "${ESTIMATION_RADIUS}" )

    LAST_P=$P
    LAST_TK2=$TK2

    P=$( echo $OUT | cut -f1 -d" " )
    TK2=$( echo $OUT | cut -f2 -d" " )

    if [ ! "${TK2}" = "0" ]
    then
        echo $( python -c "print( 'pdiff:', ($LAST_P - $P), 'diffratio:', ($LAST_P - $P)/$TK2 ) " )
    fi
done
