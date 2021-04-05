#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd )

APP_GRAPH_FLOW=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/gf-shape-evolution


h="0.125"
r="16"
a="0.015625"
O="2"
N="2"
n="8"
B="30"

TEMP_FOLDER=${SCRIPT_PATH}/temp
function compute_running_time(){
    mkdir -p ${TEMP_FOLDER}

    SHAPE="$1"
    IT="$2"

    echo "$SHAPE running time:"
    $APP_GRAPH_FLOW -S$SHAPE -h$h -r$r -a$a -i$IT -O$O -N$N -n$n -B$B ${TEMP_FOLDER}
    ENERGY_FILE="${TEMP_FOLDER}/energy.txt"

    TIME=$(grep "Execution time:"  "${ENERGY_FILE}" | cut -d":" -f2)

    MSEC=$(echo ${TIME} | grep -Po "\d+(?=ms)" )
    PER_ITERATION_MSEC=$(python3 -c "print($MSEC / $IT)")

    echo "TIME MSEC: $MSEC"
    echo "AVERAGE MSEC PER ITERATION: $PER_ITERATION_MSEC"

    rm -rf ${TEMP_FOLDER}
}

compute_running_time "triangle" "120"
compute_running_time "square" "60"
compute_running_time "flower" "150"
compute_running_time "bean" "300"
