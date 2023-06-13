
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_FOLDER="${SCRIPT_PATH}/output/didactic-one-iteration"

pushd "$(cd "${SCRIPT_PATH}" && cd .. && pwd)" > /dev/null
source "utils.sh"
popd > /dev/null

FOLDERS="S0 S-1 S+1"
COLORS=('rgb(190,216,153)' 'rgb(153,189,216)' 'rgb(216,153,189)' )
EDGES=('rgb(90,116,53)' 'rgb(53,89,116)' 'rgb(116,53,89)' )
i=-1
for FOLDER in $FOLDERS
do
  CUR="${BASE_FOLDER}/${FOLDER}"
  i="$((i+1))"
  COLOR="${COLORS[$i]}"
  echo "$COLOR"
  cd "${CUR}"
  while read -r img
  do
    # convert_to_png "${img}" "temp0.png" 
    # convert "temp0.png" -fill 'rgb(221,119,51)' -opaque 'rgb(160,160,160)' temp1.png
    # convert temp1.png -fill white -opaque black temp2.png
    # rm $img temp0.png temp1.png
    # mv temp2.png "${img%.*}.png"

    # Remove the grid lines
    sed -i '/rgb(160,160,160)/d' "${img}" 

    # convert_to_png "${img}" "${img%.*}_temp.png" 
    # convert "${img%.*}_temp.png" -fill "${COLORS[$i]}" -opaque 'rgb(190,216,153)' "${img%.*}.png"
    
    sed -i "s/rgb(190,216,153)/${COLORS[$i]}/g" "${img}"     
    sed -i "s/rgb(0,0,0)/${EDGES[$i]}/g" "${img}"     
  done < <(find . -name "*.svg")
done

