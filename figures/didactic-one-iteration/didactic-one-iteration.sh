
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_FOLDER="${SCRIPT_PATH}/output/didactic-one-iteration"

FOLDERS="S0 S-1 S+1"
for FOLDER in $FOLDERS
do
  CUR="${BASE_FOLDER}/${FOLDER}"
  cd "${CUR}"
  for img in $(ls ${CUR})
  do
    convert $img -fill 'rgb(221,119,51)' -opaque white temp.png
    convert temp.png -fill white -opaque black temp2.png
    rm $img temp.png
    mv temp2.png $img
  done
done

