#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../.. && pwd)

source ${PROJECT_PATH}/figures/utils.sh
source ${PROJECT_PATH}/plot-scripts/graph-plot.sh
pointInterval=5000

gp_plot_config()
{
	printf "set title '$1';
	set key right top;
	set xrange [10:1];
	set xlabel 'Height';
	set ylabel 'Energy';"
}

gp_add_plot()
{
    printf "'$1' u 2:3 w lp ls $3 title '$2',"
}

gp_last_plot()
{
	printf "'$1' u 2:3 w lp ls $3 title '$2';"
}

function perimeter_elastica(){
    OUTPUT_FOLDER=$1

    _DATA_FOLDER="${DATA_FOLDER}/perimeter-elastica"

    create_multiplot "${OUTPUT_FOLDER}/perimeter.eps" "Perimeter" "${_DATA_FOLDER}/connected-perimeter.txt" "Connected" \
    "${_DATA_FOLDER}/disconnected-perimeter.txt" "Disconnected"

    create_multiplot "${OUTPUT_FOLDER}/elastica.eps" "Elastica" "${_DATA_FOLDER}/connected-elastica.txt" "Connected" \
    "${_DATA_FOLDER}/disconnected-elastica.txt" "Disconnected"

}

function elastica_flexibility(){
    OUTPUT_FOLDER=$1

    _DATA_FOLDER="${DATA_FOLDER}/elastica-flexibility-alpha1"
    create_multiplot "${OUTPUT_FOLDER}/elastica-flex-a1.eps" "Elastica {/Symbol a}=1" "${_DATA_FOLDER}/connected-elastica.txt" "Connected" \
    "${_DATA_FOLDER}/disconnected-elastica.txt" "Disconnected"


    _DATA_FOLDER="${DATA_FOLDER}/elastica-flexibility-alpha05"
    create_multiplot "${OUTPUT_FOLDER}/elastica-flex-a05.eps" "Elastica {/Symbol a}=0.5" "${_DATA_FOLDER}/connected-elastica.txt" "Connected" \
    "${_DATA_FOLDER}/disconnected-elastica.txt" "Disconnected"

}

DATA_FOLDER="${SCRIPT_PATH}/data"
OUTPUT_FOLDER="${SCRIPT_PATH}/output/plot"

mkdir -p "${OUTPUT_FOLDER}"

perimeter_elastica ${OUTPUT_FOLDER}
elastica_flexibility ${OUTPUT_FOLDER}


#convert_to_png ${OUTPUT_FILEPATH} ${OUTPUT_FOLDER}/elastica.png
