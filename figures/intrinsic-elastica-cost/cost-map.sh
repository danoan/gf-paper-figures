SCRIPT_PATH="$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)"
PROJECT_PATH="$(cd "${SCRIPT_PATH}" && cd ../../ && pwd)"

INTRINSIC_COST_APP="${PROJECT_PATH}/cmake-build-release/figures/intrinsic-elastica-cost/intrinsic-elastica-cost"
SHAPES="ball bean triangle square ellipse flower"

MAIN_DATA_FOLDER="${SCRIPT_PATH}/data/cost-map"
MAIN_OUTPUT_FOLDER="${SCRIPT_PATH}/output/intrinsic-elastica-cost/cost-map"

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
    DATA_FOLDER=${MAIN_DATA_FOLDER}/${PREFIX}
    OUTPUT_FOLDER=${MAIN_OUTPUT_FOLDER}/${PREFIX}
    
    $INTRINSIC_COST_APP -S$S -K$K -h$h -w$w -a$a -b$b $O -i1 -d ${DATA_FOLDER}/${S}
    inkscape ${DATA_FOLDER}/${S}/costFunction.svg --export-pdf=${DATA_FOLDER}/${S}.pdf
    rm -rf ${DATA_FOLDER}/${S}

    mkdir -p ${OUTPUT_FOLDER}
    cp ${DATA_FOLDER}/${S}.pdf ${OUTPUT_FOLDER}/${S}.pdf
  done
}

function CostMapStudy(){
  h="0.5"
  w="8"

  ESTIMATORS="MDCA II"
  for E in $ESTIMATORS
  do
    runForShapes $E $h $w 1 0 " "  "$E/perimeter-only"

    runForShapes $E $h $w 0 1 "-O" "$E/alpha-0/squared-curvature-only"
    runForShapes $E $h $w 0 1 " "  "$E/alpha-0/squared-curvature-times-length"
    
    runForShapes $E $h $w 0.01 1 " "  "$E/elastica-a0.01"
  done
  
}

CostMapStudy