#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../ && pwd)


DATA_FOLDER=$1
FRAMES_FOLDER=$2
NAME=$3
OUTPUT_FILEPATH=$4

NFRAMES=$( ls ${FRAMES_FOLDER}/plot-frames | wc -w )

SINGLE_FOLDER=${SCRIPT_PATH}/output/${NAME}/single
mkdir -p $SINGLE_FOLDER

i=0
while [ $i -lt $NFRAMES ]
do
    pi=$( printf "%04d" $i )
    convert "${FRAMES_FOLDER}/plot-frames/animation-frame-${pi}.png" "${DATA_FOLDER}/${pi}.png" +append "${SINGLE_FOLDER}/${pi}.png"
    i=$(( i+1 ))
done


convert -delay 50 -compress LZW ${SINGLE_FOLDER}/*.png ${OUTPUT_FILEPATH}

#rm -rf ${FRAMES_FOLDER}
#rm -rf ${SCRIPT_PATH}/output/${NAME}
