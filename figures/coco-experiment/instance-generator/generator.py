from __init__ import *
import subprocess,sys,os,time

from param_combinator import *
from config import *
from template_render import *

from pycocotools.coco import COCO
import matplotlib.pyplot as plt
import skimage.io as io

APP_CONTOUR_CORRECTION="set in read  input"
SCRIPT_FOLDER="set in read  input"
OUTPUT_FOLDER="set in read  input"
IDS="set in read  input"

def resolve_output_folder(c):
    output_folder=OUTPUT_FOLDER
    for e in c:
        output_folder += "/data/" + e['path']

    return output_folder


def read_input():
    if len(sys.argv)<3:
        print("Parameters missing! COCO_DATA_DIR  COCO_DATA_TYPE OUTPUT_FOLDER")
        exit(1)

    global COCO_DATA_DIR, COCO_DATA_TYPE,OUTPUT_FOLDER,IDS
    COCO_DATA_DIR=sys.argv[1]
    COCO_DATA_TYPE=sys.argv[2]
    OUTPUT_FOLDER=sys.argv[3]
    IDS=[ int(id) for id in sys.argv[4:] ]

def main():
    read_input()
    CONFIG_LIST=[ Param("Image ID","","image_id",IDS,resolve_int)]

    annFile='{}/annotations/instances_{}.json'.format(COCO_DATA_DIR,COCO_DATA_TYPE)
    coco=COCO(annFile)

    for c in combinations(CONFIG_LIST):
        id = c[0]["value"]

        img = None
        if id==200000:
            img = coco.loadImgs([44699])[0]
        else:
            img = coco.loadImgs([id])[0]

        I = io.imread(img['coco_url'])
        plt.clf()
        plt.imshow(I); plt.axis('off')
        annIds = coco.getAnnIds(imgIds=img['id'],iscrowd=None)

        anns = coco.loadAnns(annIds)
        coco.showAnns(anns)

        plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
        plt.savefig("{}/coco-annotation.png".format(resolve_output_folder(c)),
        bbox_inches='tight',
        pad_inches=0)

    render_template("coco-exp",CONFIG_LIST,OUTPUT_FOLDER)

if __name__=='__main__':
    main()
