#! /usr/bin/env bash

SCRIPT_PATH="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="${SCRIPT_PATH%gf-paper-figures*}gf-paper-figures"

INPUT_FOLDER="${SCRIPT_PATH}/input"
OUTPUT_FOLDER="${SCRIPT_PATH}/output/hausdorff-distance"
mkdir -p "${OUTPUT_FOLDER}"

function collect_data()
{
    SHAPE="${1}"
    LEN_PEN="${2}"
    GRID_STEP="${3}"

    RADII=(3 4 5 6 7 8 9 10)
    
    PATH_PREFIX="${SCRIPT_PATH}/flow-best-of-neighborhood/output"
    PATH_SUFFIX="opt_band_2/neigh_size_2/len_pen_${LEN_PEN}/gs_${GRID_STEP}"
    
    HAUSDORFF_DISTANCE_BIN="${PROJECT_PATH}/install/bin/hausdorff-distance"
    
    # OUTPUT_DATA="${OUTPUT_FOLDER}/radial-plot-data-${SHAPE}.tsv"
    OUTPUT_DATA="${OUTPUT_FOLDER}/linear-plot-data-${SHAPE}.tsv"
    
    touch "${OUTPUT_DATA}"
    rm "${OUTPUT_DATA}"
    touch "${OUTPUT_DATA}"
    
    for RADIUS in "${RADII[@]}"
    do
        SHAPE_IMAGE_PATH="${PATH_PREFIX}/${SHAPE}/radius_${RADIUS}/${PATH_SUFFIX}"
    
        while read -r PNG_IMAGE_PATH
        do
            MIN_HD="$("${HAUSDORFF_DISTANCE_BIN}" "${PNG_IMAGE_PATH}" "${GRID_STEP}" | cut -d";" -f3)"
        done < <(find "${SHAPE_IMAGE_PATH}" -name "*.png" | sort | tail -n1 )
    
        # printf "$(( (${RADIUS}-3)*45 ))\t${MIN_HD}\n" >> "${OUTPUT_DATA}"
        printf "$(( ${RADIUS} ))\t${MIN_HD}\n" >> "${OUTPUT_DATA}"
    done

}

pushd "${SCRIPT_PATH}" > /dev/null

    # "${SCRIPT_PATH}/flow-best-of-neighborhood/exp-flow-best-of-neigh.sh"

    # SHAPES=("triangle" "square" "flower" "bean")
    # for SHAPE in "${SHAPES[@]}"
    # do
    #     echo "Collecting data for ${SHAPE}..."
    #     collect_data "${SHAPE}" 0.00098 0.12500
    # done

    # source "radial-plot.sh"
    source "linear-plot.sh"

    # create_multiplot "${OUTPUT_FOLDER}/hausdorff-distance-plot.ps" "Hausdorff Distance" \
    # "${OUTPUT_FOLDER}/radial-plot-data-triangle.tsv" "triangle" \
    # "${OUTPUT_FOLDER}/radial-plot-data-square.tsv" "square" \
    # "${OUTPUT_FOLDER}/radial-plot-data-flower.tsv" "flower" \
    # "${OUTPUT_FOLDER}/radial-plot-data-bean.tsv" "bean"
    
    create_multiplot "${OUTPUT_FOLDER}/hausdorff-distance-plot.ps" "Hausdorff Distance" \
    "${OUTPUT_FOLDER}/linear-plot-data-triangle.tsv" "triangle" \
    "${OUTPUT_FOLDER}/linear-plot-data-square.tsv" "square" \
    "${OUTPUT_FOLDER}/linear-plot-data-flower.tsv" "flower" \
    "${OUTPUT_FOLDER}/linear-plot-data-bean.tsv" "bean"

    source "${PROJECT_PATH}/figures/utils.sh"
    convert_to_png "${OUTPUT_FOLDER}/hausdorff-distance-plot.ps" "${OUTPUT_FOLDER}/hausdorff-distance-plot.png"
    rm "${OUTPUT_FOLDER}/hausdorff-distance-plot.ps"
    
popd > /dev/null

