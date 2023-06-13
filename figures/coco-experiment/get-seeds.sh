#! /usr/bin/env bash

SCRIPT_PATH="$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )"
PROJECT_PATH="$( cd "${SCRIPT_PATH}" && cd ../../.. && pwd )"

#<IMAGE_ID>
#  /noData
#    /seeds.png
#  /withData
#  gc-mask-seg.png
DATA_TEMP_FOLDER="${SCRIPT_PATH}/report/temp"
OUTPUT_FOLDER="${SCRIPT_PATH}/seeds"

mkdir -p "${OUTPUT_FOLDER}"
for FOLDER in $(ls "${DATA_TEMP_FOLDER}")
do
  cp "${DATA_TEMP_FOLDER}/${FOLDER}/noData/seeds.png" \
  "${OUTPUT_FOLDER}/seeds_${FOLDER}.png"
done



