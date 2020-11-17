#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../.. && pwd)

LEGNTH_RATIO_APP="${PROJECT_PATH}/cmake-build-release/figures/dynamic-radius/length-ratio-decrease/length-ratio-decrease"
GENERATE_DISK_APP="${PROJECT_PATH}/cmake-build-release/figures/dynamic-radius/limit-radius/generate-disk"
GRAPH_FLOW_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/graph-flow-app"

DISK_RADIUS=$1
H=$2
EVOLUTION_RADIUS=$3
ESTIMATION_RADIUS=$4

OUTPUT_FOLDER="${SCRIPT_PATH}/output"
mkdir -p ${OUTPUT_FOLDER}

DISK_IMAGE="${OUTPUT_FOLDER}/disk.png"
${GENERATE_DISK_APP} "${DISK_RADIUS}" "${DISK_IMAGE}" ${H}
${GRAPH_FLOW_APP} -S"${DISK_IMAGE}" -h${H} -a0 -b1 -r"${EVOLUTION_RADIUS}" -N0 -O2 -i15 -s ${OUTPUT_FOLDER}/temp

P="0"
TK2="0"
for F in $(ls ${OUTPUT_FOLDER}/temp/*.png)
do
    OUT=$( ${LEGNTH_RATIO_APP} "${F}" "${ESTIMATION_RADIUS}" "${H}" )

    LAST_P=$P
    LAST_TK2=$TK2

    P=$( echo $OUT | cut -f1 -d" " )
    TK2=$( echo $OUT | cut -f2 -d" " )

    echo "$OUT"
    if [ ! "${TK2}" = "0" ]
    then
        echo $( python3 -c "print( 'pdiff:', ($LAST_P - $P), 'diffratio:', ($LAST_P - $P)/$TK2 ) " )
    fi
done

rm -rf ${OUTPUT_FOLDER}/temp

#./figures/length-ratio-decrease/length-ratio-decrease.sh 50 0.0625 42 5
#314.135 0.144922
#('pdiff:', -314.135, 'diffratio:', -2167.6143028663696)
#313.98  0.14488
#('pdiff:', 0.15499999999997272, 'diffratio:', 1.069850911098652)
#313.857 0.145197
#('pdiff:', 0.12299999999999045, 'diffratio:', 0.8471249405978805)
#313.709 0.145015
#('pdiff:', 0.14800000000002456, 'diffratio:', 1.0205840775093924)
#313.571 0.145073
#('pdiff:', 0.1379999999999768, 'diffratio:', 0.9512452351573125)
#313.423 0.145296
#('pdiff:', 0.14800000000002456, 'diffratio:', 1.0186102852110488)
#313.266 0.14517
#('pdiff:', 0.15699999999998226, 'diffratio:', 1.0814906661154664)
#313.128 0.145293
#('pdiff:', 0.13800000000003365, 'diffratio:', 0.9498048770417958)
#312.988 0.145683
#('pdiff:', 0.13999999999998636, 'diffratio:', 0.9609906440695644)
#312.842 0.145229
#('pdiff:', 0.146000000000015, 'diffratio:', 1.005308857046561)
#312.693 0.145487
#('pdiff:', 0.1490000000000009, 'diffratio:', 1.0241464873150241)
#312.558 0.145612
#('pdiff:', 0.1349999999999909, 'diffratio:', 0.9271213910940782)
#312.408 0.145492
#('pdiff:', 0.14999999999997726, 'diffratio:', 1.030984521485561)
#312.256 0.145653
#('pdiff:', 0.15200000000004366, 'diffratio:', 1.0435761707623163)
#312.1   0.145862
#('pdiff:', 0.15599999999994907, 'diffratio:', 1.0695040517746162)
#311.974 0.145651
#('pdiff:', 0.1260000000000332, 'diffratio:', 0.8650815991653555)
