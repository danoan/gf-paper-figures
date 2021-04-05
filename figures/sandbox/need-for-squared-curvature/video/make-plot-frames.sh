#!/usr/bin/env bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../../ && pwd)

get_extreme()
{
    FILE=$1
    MAX=$2

    m1=$( awk '{ print $3 }' $FILE | tail -n+2 )
    m2="${m} $( awk '{ print $4 }' $FILE | tail -n+2 )"

    temp_max_file="temp_max_file.txt"
    touch ${temp_max_file}

    for a in "${m1}"
    do
        echo "${a}" >> ${temp_max_file}
    done

    for a in "${m2}"
    do
        echo "${a}" >> ${temp_max_file}
    done

    if [ $MAX = "TRUE" ]
    then
        m="$(sort ${temp_max_file} -r | head -n1 )"
    else
        m="$(sort ${temp_max_file} | head -n1 )"
    fi

    rm ${temp_max_file}
    echo $m

}

DATA_FOLDER="$1"
EXPERIMENT="$2"
CACHE_TEMP_PATH="$4"

DATA_FILE="${DATA_FOLDER}/energy.txt"

GVFM_PATH="${SCRIPT_PATH}/ext/GVFM-master"
FRAME_OUTPUT_PATH="${SCRIPT_PATH}/data/${EXPERIMENT}/plot-frames"

NFRAMES=$( wc $DATA_FILE -l | cut -d" " -f1 )
NFRAMES=$(( NFRAMES-2 ))

MAX_Y=$(get_extreme $DATA_FILE "TRUE")
MIN_Y=$(get_extreme $DATA_FILE "FALSE")

HEIGHT=$(identify ${DATA_FOLDER}/0000.png | cut -d" " -f3 | cut -d"x" -f2)

echo "Creating plot frames..."
mkdir -p ${FRAME_OUTPUT_PATH}
gnuplot -e "LOAD_PATH=\"${GVFM_PATH}\";DATA_FILE=\"${DATA_FILE}\";TITLE='${EXPERIMENT^}';START_FRAME=0;END_FRAME=${NFRAMES};MAX_Y=${MAX_Y};MIN_Y=${MIN_Y};HEIGHT=${HEIGHT};OUTPUT_PATH=\"${FRAME_OUTPUT_PATH}\"" ${GVFM_PATH}/gvfm.gp
