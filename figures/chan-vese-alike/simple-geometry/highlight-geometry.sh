#!/usr/bin/env bash

SCRIPT_FOLDER="$( cd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd)"
PROJECT_FOLDER="$(cd "${SCRIPT_FOLDER}" && cd ../../.. && pwd)"

source "${PROJECT_FOLDER}/figures/chan-vese-alike/highlight-contour.sh"
source "${PROJECT_FOLDER}/figures/utils.sh"

HIGHLIGHT_CONTOUR_APP="${PROJECT_FOLDER}/cmake-build-release/figures/chan-vese-alike/highlight-contour"
BASE_IMAGE="${SCRIPT_FOLDER}/input/simple-geometry-noisy.pgm"
DATA_FOLDER="${SCRIPT_FOLDER}/data/noisy/curvature/2"
OUTPUT_FOLDER="${PROJECT_FOLDER}/figures/chan-vese-alike/output/chan-vese-alike/simple-geometry"

highlight_contour "${HIGHLIGHT_CONTOUR_APP}" "${BASE_IMAGE}" "${DATA_FOLDER}" "${OUTPUT_FOLDER}"
mogrify -crop 120x120+90+90 "${OUTPUT_FOLDER}/contours.png"

convert_to_png "${BASE_IMAGE}" "${OUTPUT_FOLDER}/simple-geometry-noisy.png"
mogrify -crop 120x120+90+90 "${OUTPUT_FOLDER}/simple-geometry-noisy.png"