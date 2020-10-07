#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd )

ZERO_LEVEL_SET=${PROJECT_PATH}/cmake-build-release/figures/zero-level-set/zero-level-set
OUTPUT_FOLDER=${SCRIPT_PATH}/output/zero-level-set

${ZERO_LEVEL_SET} flower 0.125 32 33 ${OUTPUT_FOLDER}/flower.eps
epstopdf ${OUTPUT_FOLDER}/flower.eps --pdfsettings=screen
rm ${OUTPUT_FOLDER}/flower.eps
convert -density 100 -depth 8 -quality 85 ${OUTPUT_FOLDER}/flower.pdf ${OUTPUT_FOLDER}/balance-coefficient-zero-level-set.png
rm ${OUTPUT_FOLDER}/flower.pdf




