#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../.. && pwd)

GENERATE_DISK_APP="${PROJECT_PATH}/cmake-build-release/figures/dynamic-radius/limit-radius/generate-disk"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app"
LIMIT_RADIUS_APP="${SCRIPT_PATH}/limit-radius.sh"
OUTPUT_FOLDER="${SCRIPT_PATH}/output"

mkdir -p ${OUTPUT_FOLDER}

DATA_OUT_FILE="${OUTPUT_FOLDER}/limit_radius_data.txt"
echo "#Radius LimitRadius" > $DATA_OUT_FILE

MIN_RADIUS=3
MAX_RADIUS=12

MAX_EVOLUTIONS=5
RADIUS=$MIN_RADIUS
while [ $RADIUS -le $MAX_RADIUS ]
do
    LR=$( $LIMIT_RADIUS_APP $RADIUS $MAX_EVOLUTIONS)
    echo "$RADIUS $LR" >> $DATA_OUT_FILE
    RADIUS=$(( $RADIUS + 1))
done
 
source ${PROJECT_PATH}/figures/utils.sh
source ${PROJECT_PATH}/plot-scripts/graph-plot.sh
pointInterval=5000

gp_plot_config()
{
	printf "set title '$1';
	set key right center;
	set yrange[0:];
	set xlabel 'Balance coefficient radius';
	set ylabel 'Limit radius';"
}

gp_last_plot()
{
	printf "'$1' u 1:2 w lp ls $3 title '$2';"
}

create_multiplot "${OUTPUT_FOLDER}/limit-radius-plot.eps" "Smallest disk such that evolution stops in fewer than 3 iterations" "$DATA_OUT_FILE" "disk" 