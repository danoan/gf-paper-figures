#!/bin/bash
SCRIPT_FOLDER="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_FOLDER="$(cd "$SCRIPT_FOLDER" && cd ../../.. && pwd)"

GRAPH_SEG_BIN="${PROJECT_FOLDER}/ext-projects/cmake-build-release/bin"

usage(){ echo "Usage: $0 "
echo "[-R Balance coefficient radius (default: 7)]"
echo "[-r Validation disk radius (default: 16)]"
echo "[-G Data term penalization at candidate selection (default: 0.5) ]"
echo "[-K Curvature term penalization at candidate selection (default: 1.0) ]"
echo "[-g Data term penalization at validation (default: 2.0) ]"
echo "[-k Curvature term penalization at validation (default: 1.0) ]"
echo "[-a Length term penalization at validation (default:0.01) ]"
echo "[-O Optimization band (default:2)]"
echo "[-N neighborhood size (default:2)]"
echo "[-H neighborhood type (morphology, random) (default:morphology)]"
echo "[-j Grabcut iterations (default:1)]"
echo "[-i Max iterations (default:30) ]"
echo "[-t Tolerance. Stop if change between iterations is smaller than tolerance (>=0) (default:-1, stop when max iterations is reached)]\n"
echo "[-n Number of threads (default:4) ]"
echo "[-v Validation weight mode (automatic-correction-length-data, automatic-correction-length, automatic-correction-data, manual) (default: automatic-correction-length-data)\n"
echo "[-w Print energy value ]"
echo "[-s Save all figures ]"
echo "[-I INPUT_IMAGE_PATH ]"
echo " OUTPUT_FOLDER" 1>&2; exit 1;}

R="7"
r="16"
G="0.5"
K="1.0"
g="2.0"
k="1.0"
a="0.01"
O="2"
N="2"
H="morphology"
j="1"
i="30"
t="-1"
w=""
s=""
n="4"
v="automatic-correction-data"
while getopts ":R:r:G:K:g:k:a:O:N:H:j:i:t:n:v:I:ws" o; do
    case "${o}" in
        R)
            R=$OPTARG
            ;;
        r)
            r=$OPTARG
            ;;
        G)
            G=$OPTARG
            ;;
	    K)
	        K=$OPTARG
	        ;;
        g)
            g=$OPTARG
            ;;
	    k)
	        k=$OPTARG
	        ;;
	    a)
	        a=$OPTARG
	        ;;
	    O)
	        O=$OPTARG
	        ;;
	    N)
	        N=$OPTARG
	        ;;
	    H)
	        H=$OPTARG
	        ;;
	    j)
	        j=$OPTARG
	        ;;
	    i)
	        i=$OPTARG
	        ;;
	    t)
	        t=$OPTARG
	        ;;
	    n)
	        n=$OPTARG
	        ;;
	    v)
	        v=$OPTARG
	        ;;
	    I)
	        INPUT_IMAGE=$OPTARG
	        ;;
	    w)
	        w="-w"
	        ;;
	    s)
	        s="-s"
	        ;;
        :)
            echo "Invalid option: $OPTARG requires an argument" 1>&2
            exit 1;
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND -1))

OUTPUT_FOLDER="$1"
mkdir -p "$OUTPUT_FOLDER"

if [ -z "$OUTPUT_FOLDER" ]
then
    usage
    exit
fi

SEED_SELECTOR_APP="${GRAPH_SEG_BIN}/seed-selector"
GRAB_CUT_APP="${GRAPH_SEG_BIN}/grabcut"
GRAPH_SEG_APP="${GRAPH_SEG_BIN}/gf-segmentation"


IMAGE_NAME="$(basename $INPUT_IMAGE)"
IMAGE_NAME="${IMAGE_NAME%%.*}"

SP_OUT="${OUTPUT_FOLDER}/${IMAGE_NAME}"
mkdir -p "$SP_OUT"


echo "Output files in: $SP_OUT"
echo ""

SEED_SELECTOR_OUT=""

if [ ! -f "${SP_OUT}/mask-bg-0.pgm" ]
then
    "${SEED_SELECTOR_APP}" "${INPUT_IMAGE}" "${OUTPUT_FOLDER}" -o
    if [ $? -eq 0 ]
    then
        exit
    fi

    "${GRAB_CUT_APP}" "${INPUT_IMAGE}" "${SP_OUT}/mask-fg-0.pgm" \
    "${SP_OUT}/mask-bg-0.pgm" \
    "${SP_OUT}/gc-object.xml" \
    -u "${SP_OUT}/mask-pbfg-0.pgm" -d

    "${GRAPH_SEG_APP}" "${SP_OUT}/gc-object.xml" -R"$R" -r"$r" -G"$G" -g"$g" -K"$K" -k"$k" -a"$a" -N"${N}" -H"${H}" -O"${O}" -j"${j}" -i"$i" -t"$t" -n${n} -v"${v}" -d ${s} ${w} "${SP_OUT}/graph-seg"
fi

while :
do
    "${SEED_SELECTOR_APP}" "${INPUT_IMAGE}" "$OUTPUT_FOLDER" \
    -u "${SP_OUT}/mask-pbfg-0.pgm" \
    -f "${SP_OUT}/mask-fg-0.pgm" \
    -b "${SP_OUT}/mask-bg-0.pgm" \
    -s "${SP_OUT}/graph-seg/mask-seg.png" -o

    if [ $? -eq 0 ]
    then
        exit
    fi

    "${GRAB_CUT_APP}" "${INPUT_IMAGE}" "${SP_OUT}/mask-fg-0.pgm" "${SP_OUT}/mask-bg-0.pgm" "${SP_OUT}/gc-object.xml" \
    -u "${SP_OUT}/mask-pbfg-0.pgm" -s "${SP_OUT}/graph-seg/mask-seg.png"

    "${GRAPH_SEG_APP}" "${SP_OUT}/gc-object.xml" -R"$R" -r"$r" -G"$G" -g"$g" -K"$K" -k"$k" -a"$a" -N"${N}" -H"${H}" -O"${O}" -j"${j}" -i"$i" -t"$t" -n${n} -v"${v}" -d ${s} ${w} "${SP_OUT}/graph-seg"
done
