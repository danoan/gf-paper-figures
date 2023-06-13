#!/usr/bin/env bash

function convert_to_png(){
    INPUT_FILE=$1
    OUTPUT_FILE=$2

    convert -background none -density 100 -depth 32 -quality 100 ${INPUT_FILE} ${OUTPUT_FILE}
}
