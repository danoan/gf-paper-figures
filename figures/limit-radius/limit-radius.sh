#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

GENERATE_DISK_APP="${PROJECT_PATH}/cmake-build-release/figures/limit-radius/generate-disk"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app"
OUTPUT_FOLDER="${SCRIPT_PATH}/output"

mkdir -p ${OUTPUT_FOLDER}

BALANCE_RADIUS=$1

LOWER_RADIUS=10
HIGHER_RADIUS=1000
RADIUS=$( python -c "print((${HIGHER_RADIUS} + ${LOWER_RADIUS})/2)" )

while [ $LOWER_RADIUS -lt $HIGHER_RADIUS ]
do
    echo "Lower: ${LOWER_RADIUS} -- Higher: ${HIGHER_RADIUS} -- Radius: ${RADIUS}"
    DISK_IMAGE=${OUTPUT_FOLDER}/disk.png
    ${GENERATE_DISK_APP} ${RADIUS} ${DISK_IMAGE}
    ${GRAPH_FLOW_APP} -S${DISK_IMAGE} -h1.0 -a0 -b1 -r${BALANCE_RADIUS} -N0 -O2 -i15 -s -w ${OUTPUT_FOLDER}/temp

    IMG_COUNT=$(ls ${OUTPUT_FOLDER}/temp/*.png | wc -l)
    if [ ${IMG_COUNT} -le "4" ]
    then
        HIGHER_RADIUS=${RADIUS}
        RADIUS=$( python -c "print((${HIGHER_RADIUS} + ${LOWER_RADIUS})/2)" )
    else
        LOWER_RADIUS=$(( $RADIUS + 1 ))
        RADIUS=$( python -c "print((${HIGHER_RADIUS} + ${LOWER_RADIUS})/2)" )
    fi

    rm ${OUTPUT_FOLDER}/temp/*.png

done

echo "Limit radius of ${BALANCE_RADIUS} equals ${RADIUS}"