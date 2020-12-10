SCRIPT_PATH="$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)"

function getDouble(){
  DATA_FILE="$1"
  LABEL="$2"
  VAL=$(grep "${LABEL}*." "${DATA_FILE}" | cut -d":" -f2 | grep "[\d\w\.\-]*" -o -P)

  echo "${VAL}"
}

function updateValuesInTex(){
  LATEX_FILE="$1"
  SUMMARY_FILE="$2"
  ESTIMATOR="$3"
  IDENTIFIER="$4"
  SUFFIX="$5"

  MIN_COST="$(getDouble ${SUMMARY_FILE} "Min unit cost" )"
  MAX_COST="$(getDouble ${SUMMARY_FILE} "Max unit cost" )"

  MARKER="${ESTIMATOR^^}-${IDENTIFIER^^}-${S^^}"
  sed -i "s/.*%${MARKER}/m:\$${MIN_COST}\$,M:\$${MAX_COST}\$ ${SUFFIX} %${MARKER}/g" "${LATEX_FILE}"
}

# Test 
# LATEX_FILE="${SCRIPT_PATH}/jaco-discussions.tex"
# cp "${SCRIPT_PATH}/original/jaco-discussions.tex" "${LATEX_FILE}"

# DATA_FOLDER_II="/home/daniel/Projects/Git/gf-paper-figures/figures/intrinsic-elastica-cost/output/intrinsic-elastica-cost/cost-map/II"
# DATA_FOLDER_MDCA="/home/daniel/Projects/Git/gf-paper-figures/figures/intrinsic-elastica-cost/output/intrinsic-elastica-cost/cost-map/MDCA"

LATEX_FILE="$1"
DATA_FOLDER_II="${SCRIPT_PATH}/output/intrinsic-elastica-cost/cost-map/II"
DATA_FOLDER_MDCA="${SCRIPT_PATH}/output/intrinsic-elastica-cost/cost-map/MDCA"

SHAPES="ball bean square"
for S in $SHAPES
do
  if [ $S = "square" ] 
  then
    LAST='\\\\'
  else
    LAST='\&'
  fi

  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_II}/alpha-0/squared-curvature-only/${S}.txt" "II" "squared-curvature-only" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_II}/alpha-0/squared-curvature-times-length/${S}.txt" "II" "squared-curvature-times-length" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_II}/perimeter-only/${S}.txt" "II" "perimeter-only" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_II}/elastica-a0.01/${S}.txt" "II" "elastica-a0.01" "${LAST}"

  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_MDCA}/alpha-0/squared-curvature-only/${S}.txt" "MDCA" "squared-curvature-only" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_MDCA}/alpha-0/squared-curvature-times-length/${S}.txt" "MDCA" "squared-curvature-times-length" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_MDCA}/perimeter-only/${S}.txt" "MDCA" "perimeter-only" "${LAST}"
  updateValuesInTex "${LATEX_FILE}" "${DATA_FOLDER_MDCA}/elastica-a0.01/${S}.txt" "MDCA" "elastica-a0.01"  "${LAST}"
done




