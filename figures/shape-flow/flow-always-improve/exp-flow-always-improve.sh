#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../.. && pwd )

APP_GRAPH_FLOW=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app
APP_SUMMARY_FLOW=${PROJECT_PATH}/ext-projects/cmake-build-release/bin/summary-flow

python3 ${SCRIPT_PATH}/instance-generator/generator.py ${APP_GRAPH_FLOW} ${APP_SUMMARY_FLOW} ${SCRIPT_PATH}/output