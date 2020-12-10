SCRIPT_PATH="$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)"
PROJECT_PATH="$(cd "${SCRIPT_PATH}" && cd ../../ && pwd)"

INTRINSIC_COST_APP="${PROJECT_PATH}/cmake-build-release/figures/intrinsic-elastica-cost/intrinsic-elastica-cost"
SHAPES="ball bean triangle square ellipse flower"

MAIN_DATA_FOLDER="${SCRIPT_PATH}/data/flow"
MAIN_OUTPUT_FOLDER="${SCRIPT_PATH}/output/intrinsic-elastica-cost/flow"

function runForShapes()
{  
  K="$1"
  h="$2"
  w="$3"
  a="$4"
  b="$5"
  O="$6"
  PREFIX="$7"

  for S in $SHAPES
  do
    $INTRINSIC_COST_APP -S$S -K$K -h$h -w$w -a$a -b$b $O -i27 ${MAIN_DATA_FOLDER}/${PREFIX}/${S}

    mkdir -p ${MAIN_OUTPUT_FOLDER}/${PREFIX}/${S}
    cp ${MAIN_DATA_FOLDER}/${PREFIX}/${S}/it-0.png ${MAIN_OUTPUT_FOLDER}/${PREFIX}/${S}/it-0.png
    cp ${MAIN_DATA_FOLDER}/${PREFIX}/${S}/it-0001.png ${MAIN_OUTPUT_FOLDER}/${PREFIX}/${S}/it-0001.png
    cp ${MAIN_DATA_FOLDER}/${PREFIX}/${S}/it-0010.png ${MAIN_OUTPUT_FOLDER}/${PREFIX}/${S}/it-0010.png
    cp ${MAIN_DATA_FOLDER}/${PREFIX}/${S}/it-0015.png ${MAIN_OUTPUT_FOLDER}/${PREFIX}/${S}/it-0015.png
  done
}

function FlowStudy(){
  h="0.5"
  w="4"

  ESTIMATORS="MDCA II"
  for E in $ESTIMATORS
  do
    runForShapes $E $h $w 0 1 " "  "$E/alpha-0/squared-curvature-times-length"
    runForShapes $E $h $w 0 1 "-O"  "$E/alpha-0/squared-curvature-only"

    runForShapes $E $h $w 0.01 1 " "  "$E/elastica-a0.01"
  done
}

FlowStudy