#! /bin/bash

SCRIPT_PATH="$( cd $(dirname "${BASH_SOURCE[0]}") && pwd )"
PROJECT_PATH="$( cd "${SCRIPT_PATH}" && cd ../.. && pwd )"

source "${SCRIPT_PATH}/ids.sh"

COCO_DATA_DIR="${1}"
COCO_DATA_TYPE="${2}"
OUTPUT_FOLDER="${SCRIPT_PATH}/report"

CONTOUR_CORRECTION_OUTPUT_DIR="${OUTPUT_FOLDER}/temp"

IT=20
N=3
O=4
g=1
G=1
a=10
R=5
h=1.0

function create_gc_object(){
  INTERACTIVE_SEG_APP="${SCRIPT_PATH}/util-apps/interactive-seg.sh"

  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    ${INTERACTIVE_SEG_APP} -I "${COCO_DATA_DIR}/images/${PADDED_ID}.jpg" -i${IT} \
    -j5 -R${R} -r${R} -O${O} -g${g} -G${G} -a${a} -H morphology -N${N} \
    "${OUTPUT_FOLDER}/interactive-seg"
  done
}

function run_contour_correction()
{
  CONTOUR_CORRECTION_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/gf-contour-correction"
  EXPORT_SEG_MASK_APP="${PROJECT_PATH}/install/bin/export-seg-mask"

  GC_OBJECT_DATA_DIR="${OUTPUT_FOLDER}/interactive-seg"
  mkdir -p "${CONTOUR_CORRECTION_OUTPUT_DIR}"

  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    printf "Generating results for image id %s\n" $PADDED_ID
    ${CONTOUR_CORRECTION_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/noData" -i${IT} -j5 -N${N} -R${R} -r${R} -O${O} -g0 -G0 -a${a}
    ${CONTOUR_CORRECTION_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData" -i${IT} -j5 -N${N} -R${R} -r${R} -O${O} -g${g} -G${G} -a${a}

    ${EXPORT_SEG_MASK_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/gc-mask-seg.png" 5
  done
}

function extra_contour_correction()
{
  #To illustrate how the result can be improved by tuning the parameters accordingly
  GC_OBJECT_DATA_DIR="${OUTPUT_FOLDER}/interactive-seg"

  cp -r "${GC_OBJECT_DATA_DIR}/000000044699" "${GC_OBJECT_DATA_DIR}/000000200000"
  CONTOUR_CORRECTION_APP="${PROJECT_PATH}/ext-projects/cmake-build-release/bin/gf-contour-correction"
  EXPORT_SEG_MASK_APP="${PROJECT_PATH}/install/bin/export-seg-mask"

  mkdir -p "${CONTOUR_CORRECTION_OUTPUT_DIR}"

  ID=200000
  IDS="${IDS} ${ID}"
  PADDED_ID="$(printf "%012d" ${ID})"
  printf "Generating results for image id %s\n" $PADDED_ID
  ${CONTOUR_CORRECTION_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/noData" -i${IT} -j5 -N${N} -R3 -r3 -O${O} -g0 -G0 -a${a}
  ${CONTOUR_CORRECTION_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData" -i${IT} -j5 -N${N} -R3 -r3 -O${O} -g${g} -G${G} -a${a}

  ${EXPORT_SEG_MASK_APP} "${GC_OBJECT_DATA_DIR}/${PADDED_ID}/gc-object.xml" "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/gc-mask-seg.png" 5
}

function compute_metrics()
{
  INPUT_STR=""
  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    INPUT_STR="${INPUT_STR} $ID \
    ${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData/mask-seg.png \
    ${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/noData/mask-seg.png \
    ${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/gc-mask-seg.png"
  done

  python3 ${SCRIPT_PATH}/python-scripts/compute-metrics.py ${COCO_DATA_DIR} \
  ${COCO_DATA_TYPE} \
  "${OUTPUT_FOLDER}/data/precision-recall-metrics.csv" \
  ${INPUT_STR}
}

function correction_report(){
  CORRECTION_REPORT="${PROJECT_PATH}/install/bin/correction-report"
  REPORT_FILE="${OUTPUT_FOLDER}/data/correction-report.csv"

  echo "id,inflection-gc,inflection-cc,elastica-gc,elastica-cc,area-similarity" > ${REPORT_FILE}
  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    mkdir -p "${OUTPUT_FOLDER}/data/${ID}"

    GC_MASK_PATH="${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/gc-mask-seg.png"
    CC_MASK_PATH="${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData/mask-seg.png"

    printf "${ID}," >> ${REPORT_FILE}

    ${CORRECTION_REPORT} -i"${GC_MASK_PATH}" -h${h} -e${R} -p"inflection" >> ${REPORT_FILE}
    printf "," >> ${REPORT_FILE}
    ${CORRECTION_REPORT} -i"${CC_MASK_PATH}" -h${h} -e${R} -p"inflection" >> ${REPORT_FILE}
    printf "," >> ${REPORT_FILE}

    ${CORRECTION_REPORT} -i"${GC_MASK_PATH}" -h${h} -e${R} -p"elastica" >> ${REPORT_FILE}
    printf "," >> ${REPORT_FILE}
    ${CORRECTION_REPORT} -i"${CC_MASK_PATH}" -h${h} -e${R} -p"elastica" >> ${REPORT_FILE}
    printf "," >> ${REPORT_FILE}

    ${CORRECTION_REPORT} -i"${GC_MASK_PATH}" -I${CC_MASK_PATH} -p"area-diff" >> ${REPORT_FILE}
    printf "\n" >> ${REPORT_FILE}

    #Tangent profile
    ${CORRECTION_REPORT} -i"${GC_MASK_PATH}" -h${h} -e${R} -p"tangent" -o"${OUTPUT_FOLDER}/data/${ID}/tangent-profile-gc.txt"
    ${CORRECTION_REPORT} -i"${CC_MASK_PATH}" -h${h} -e${R} -p"tangent" -o"${OUTPUT_FOLDER}/data/${ID}/tangent-profile-cc.txt"

    python3 ${SCRIPT_PATH}/python-scripts/plot-tangent-profile.py "${OUTPUT_FOLDER}/data/${ID}/tangent-profile-gc.txt" "${OUTPUT_FOLDER}/data/${ID}/tangent-profile-gc.png"
    python3 ${SCRIPT_PATH}/python-scripts/plot-tangent-profile.py "${OUTPUT_FOLDER}/data/${ID}/tangent-profile-cc.txt" "${OUTPUT_FOLDER}/data/${ID}/tangent-profile-cc.png"
  done
}

function create_plots()
{
  SCATTER_PLOT_APP="${SCRIPT_PATH}/python-scripts/scatter-box-plot.py"
  python3 ${SCATTER_PLOT_APP} \
  "${OUTPUT_FOLDER}/data/precision-recall-metrics.csv" \
  "${OUTPUT_FOLDER}/data/correction-report.csv" \
  "${OUTPUT_FOLDER}/data"
}

function create_report(){
  mkdir -p "${OUTPUT_FOLDER}/data"

  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    cp "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/noData/corrected-seg.png" "${OUTPUT_FOLDER}/data/${ID}/corrected-seg-without-data.png"
    cp "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData/corrected-seg.png" "${OUTPUT_FOLDER}/data/${ID}/corrected-seg-with-data.png"
    cp "${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData/gc-seg.png" "${OUTPUT_FOLDER}/data/${ID}/gc-seg.png"
  done

  python3 "${SCRIPT_PATH}/instance-generator/generator.py" "${COCO_DATA_DIR}" "${COCO_DATA_TYPE}" "${OUTPUT_FOLDER}" ${IDS}
}

function running_time_table(){
  HIGHEST_MSEC=0
  LOWEST_MSEC=100000
  AVERAGE_MSEC=0
  PER_ITERATION_MSEC=0
  N_IDS=0
  for ID in ${IDS}
  do
    PADDED_ID="$(printf "%012d" ${ID})"
    ENERGY_FILE="${CONTOUR_CORRECTION_OUTPUT_DIR}/${PADDED_ID}/withData/energy.txt"
    TIME=$(grep "Execution time:"  "${ENERGY_FILE}" | cut -d":" -f2)
    MSEC=$(echo ${TIME} | grep -Po "\d+(?=ms)" )

    if [ $LOWEST_MSEC -gt $MSEC ]
    then
      LOWEST_MSEC=$MSEC
    fi

    if [ $HIGHEST_MSEC -lt $MSEC ]
    then
      HIGHEST_MSEC=$MSEC
    fi

    AVERAGE_MSEC=$(( $AVERAGE_MSEC + $MSEC))
    N_ID=$(($N_ID+1))
  done

  AVERAGE_MSEC=$(python3 -c "print($AVERAGE_MSEC / $N_ID)")
  PER_ITERATION_MSEC=$(python3 -c "print($AVERAGE_MSEC / 20)")
  echo "HIGHEST MSEC: $HIGHEST_MSEC"
  echo "LOWEST MSEC: $LOWEST_MSEC"
  echo "AVERAGE MSEC: $AVERAGE_MSEC"
  echo "AVERAGE MSEC PER ITERATION: $PER_ITERATION_MSEC"
}

function copy_paper_figures(){
  KITE="${OUTPUT_FOLDER}/data/7784"
  MOTO="${OUTPUT_FOLDER}/data/102331"
  GIRAFFE="${OUTPUT_FOLDER}/data/17031"
  BASEBALL="${OUTPUT_FOLDER}/data/123213"
  SHEEP_R5="${OUTPUT_FOLDER}/data/44699"
  SHEEP_R3="${OUTPUT_FOLDER}/data/200000"
  CUP="${OUTPUT_FOLDER}/data/2592"
  ZEBRA="${OUTPUT_FOLDER}/data/54123"
  TRAIN="${OUTPUT_FOLDER}/data/135902"

  BASE_OUTPUT="${SCRIPT_PATH}/output/coco-experiment"

  SAMPLE_IMAGES="${BASE_OUTPUT}/sample-images"
  mkdir -p ${SAMPLE_IMAGES}

  cp -rT ${KITE} ${SAMPLE_IMAGES}/kite
  cp -rT ${MOTO} ${SAMPLE_IMAGES}/moto
  cp -rT ${GIRAFFE} ${SAMPLE_IMAGES}/giraffe

  TANGENT_PROFILE="${BASE_OUTPUT}/tangent-profile"
  mkdir -p ${TANGENT_PROFILE}

  cp -rT ${BASEBALL} ${TANGENT_PROFILE}/baseball

  PARAMETER_TUNNING="${BASE_OUTPUT}/parameter-tuning"
  mkdir -p ${PARAMETER_TUNNING}

  cp -rT ${SHEEP_R3} ${PARAMETER_TUNNING}/sheep_r3
  cp -rT ${SHEEP_R5} ${PARAMETER_TUNNING}/sheep_r5

  COMPLETION="${BASE_OUTPUT}/completion"
  mkdir -p ${COMPLETION}

  cp -rT ${CUP} ${COMPLETION}/cup
  cp -rT ${ZEBRA} ${COMPLETION}/zebra
  cp -rT ${TRAIN} ${COMPLETION}/train

  cp ${OUTPUT_FOLDER}/data/box-plot-correction.png ${BASE_OUTPUT}/box-plot-correction.png
  cp ${OUTPUT_FOLDER}/data/box-plot-mixed.png ${BASE_OUTPUT}/box-plot-mixed.png
}


function clean(){
  rm -rf "${CONTOUR_CORRECTION_OUTPUT_DIR}/temp"
}


create_gc_object
run_contour_correction
extra_contour_correction
compute_metrics
correction_report
create_plots
create_report
running_time_table
copy_paper_figures

# clean
