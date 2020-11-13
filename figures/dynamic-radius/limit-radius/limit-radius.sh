#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../.. && pwd)

GENERATE_DISK_APP="${PROJECT_PATH}/cmake-build-release/figures/dynamic-radius/limit-radius/generate-disk"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app"
OUTPUT_FOLDER="${SCRIPT_PATH}/output"

mkdir -p ${OUTPUT_FOLDER}

VERBOSE_FLAG=0
while getopts ":v" o; do
    case "${o}" in
        v)
            VERBOSE_FLAG=1
            ;;
        :)
            echo "Invalid option: $OPTARG requires an argument" 1>&2
            exit 1;
            ;;
        *)
            usage
            ;;
    esac
done

shift $((OPTIND -1))
BALANCE_RADIUS=$1
MAX_EVOLUTIONS=$2


cond_print(){
    if [ $VERBOSE_FLAG = 1 ] 
    then
        echo "$1"
    fi
}



LOWER_RADIUS=10
HIGHER_RADIUS=1500
RADIUS=$( python3 -c "print(int( (${HIGHER_RADIUS} + ${LOWER_RADIUS})/2))" )

while [ $LOWER_RADIUS -lt $HIGHER_RADIUS ]
do
    cond_print "Lower: ${LOWER_RADIUS} -- Higher: ${HIGHER_RADIUS} -- Radius: ${RADIUS}" 
    
    DISK_IMAGE=${OUTPUT_FOLDER}/disk.png
    ${GENERATE_DISK_APP} ${RADIUS} ${DISK_IMAGE} 1.0

    if [ $VERBOSE_FLAG = 1 ]
    then
        ${GRAPH_FLOW_APP} -S${DISK_IMAGE} -h1.0 -a0 -b1 -r${BALANCE_RADIUS} -N0 -O2 -i15 -t0 -s -w ${OUTPUT_FOLDER}/temp
    else
        ${GRAPH_FLOW_APP} -S${DISK_IMAGE} -h1.0 -a0 -b1 -r${BALANCE_RADIUS} -N0 -O2 -i15 -t0 -s ${OUTPUT_FOLDER}/temp
    fi

    IMG_COUNT=$(ls ${OUTPUT_FOLDER}/temp/*.png | wc -l)
    if [ ${IMG_COUNT} -le "$MAX_EVOLUTIONS" ]
    then
        HIGHER_RADIUS=${RADIUS}
        RADIUS=$( python3 -c "print( int( (${HIGHER_RADIUS} + ${LOWER_RADIUS})/2) )" )
    else
        LOWER_RADIUS=$(( $RADIUS + 1 ))
        RADIUS=$( python3 -c "print( int( (${HIGHER_RADIUS} + ${LOWER_RADIUS})/2) )" )
    fi

    rm ${OUTPUT_FOLDER}/temp/*.png

done

if [ $VERBOSE_FLAG = 1 ]
then
    echo "Limit radius of ${BALANCE_RADIUS} equals ${RADIUS}"
else
    echo "${RADIUS}"
fi