#!/usr/bin/env bash

function get_color(){
    #BGR
    _PINK="255 0 255"
    _GREEN="100 255 0"
    _YELLOW="0 255 255"
    _RED="0 255 0"

    _N=$1
    if [ $_N -eq "0" ]
    then
        echo $_PINK
    elif [ $_N -eq "1" ]
    then
        echo $_GREEN
    elif [ $_N -eq "2" ]
    then
        echo $_YELLOW
    else
        echo $_RED
    fi
}

function highlight_contour(){
    _HIGHLIGHT_CONTOUR_APP="$1"
    _BASE_IMAGE="$2"
    _DATA_FOLDER="$3"
    _OUTPUT_FOLDER="$4"

    mkdir -p ${_OUTPUT_FOLDER}

    _TOTAL=$(ls ${_DATA_FOLDER}/*.png | wc -l)
    _FINAL="$(printf "%04d.png" $(( ${_TOTAL}-3 )))"
    _MIDDLE=$( python -c "print( ${_TOTAL}//3 )" )
    _MIDDLE="$(printf "%04d.png" ${_MIDDLE})"
    _INITIAL="0000.png"

    echo $_MIDDLE

    _it=0
    _CONTOURS="$_INITIAL $_MIDDLE $_FINAL"
    for _CONTOUR in $_CONTOURS
    do
        _OUTPUT_IMAGE="${_OUTPUT_FOLDER}/${_CONTOUR}"
        _MASK_IMAGE="${_DATA_FOLDER}/${_CONTOUR}"

        ${_HIGHLIGHT_CONTOUR_APP} ${_BASE_IMAGE} ${_MASK_IMAGE} ${_OUTPUT_IMAGE} $(get_color $_it)
        _BASE_IMAGE=${_OUTPUT_IMAGE}

        _it=$(( $_it+1 ))
    done

    rm "${_OUTPUT_FOLDER}/${_INITIAL}"
    rm "${_OUTPUT_FOLDER}/${_MIDDLE}"
    mv "${_OUTPUT_FOLDER}/${_FINAL}" "${_OUTPUT_FOLDER}/contours.png"


}


