#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd )

RING_APP="${PROJECT_PATH}/cmake-build-release/figures/ring/ring"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/cv-alike/bin/graph-flow-app"

OUTPUT_FOLDER="/home/dantu/GIT/PhD/graph-flow-paper-figures/figures/ring/output"
mkdir -p "${OUTPUT_FOLDER}"

H="1"
${RING_APP} 30 120 ${H} "${OUTPUT_FOLDER}/ring.png"
${GRAPH_FLOW_APP} -S"${OUTPUT_FOLDER}/ring.png" ${OUTPUT_FOLDER}/ring -i500 -h0.5 -d -w -s -r7 -v5 -a0.001