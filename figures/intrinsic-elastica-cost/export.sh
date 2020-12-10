#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd .. && pwd)

LATEX_FIGURES_FOLDER=$(realpath $1)
fp="intrinsic-elastica-cost"

mkdir -p ${LATEX_FIGURES_FOLDER}/$fp
cp -r ${SCRIPT_PATH}/output/$fp ${LATEX_FIGURES_FOLDER}
