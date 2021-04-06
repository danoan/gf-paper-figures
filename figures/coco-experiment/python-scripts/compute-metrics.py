from pycocotools.coco import COCO
import numpy as np
import skimage
import skimage.io as io
import skimage.color as color
import matplotlib.pyplot as plt
import pylab

import sys

DEBUG=True

def computeMetrics(maskBase,maskCompare):
  b_maskBase = maskBase>0
  b_maskCompare = maskCompare>0

  true_positive = b_maskCompare[ b_maskBase ].sum()

  false_positive =  b_maskCompare.sum() - true_positive
  false_negative = b_maskBase.sum() - true_positive

  return {
    "precision": true_positive / (true_positive + false_positive), #b_maskCompare.sum()
    "recall": true_positive / (true_positive + false_negative) #b_maskBase.sum()
   }

def findMoreLikelyAnnotation(cocoImg,maskImg):
  annIds = coco.getAnnIds(imgIds=cocoImg['id'])
  anns = coco.loadAnns(annIds)

  aRank={}
  for i,annotation in enumerate(anns):
    cocoMask = coco.annToMask(annotation)

    M = computeMetrics(cocoMask,maskImg)
    aRank[i] = M["precision"] + M["recall"]

  highestMatch = sorted(aRank.items(),key=lambda item: item[1],reverse=True)[0]
  if DEBUG:
    print("Highest Match: ",highestMatch)

  return anns[ highestMatch[0] ]

def compareMaskWithCocoMask(cocoImgId,maskPath):
  cocoImg = coco.loadImgs(cocoImgId)[0]

  maskImg = io.imread(maskPath,as_gray=True)
  maskImg = skimage.img_as_bool(maskImg)

  ann = findMoreLikelyAnnotation(cocoImg,maskImg)
  cocoMask = coco.annToMask(ann)

  alo = color.gray2rgb(cocoMask)
  alo*=255

  return computeMetrics(cocoMask,maskImg)

COCO_DATA_DIR=sys.argv[1]
COCO_DATA_TYPE=sys.argv[2]
OUTPUT_METRICS_FILE_PATH=sys.argv[3]

annFile='{}/annotations/instances_{}.json'.format(COCO_DATA_DIR,COCO_DATA_TYPE)
coco=COCO(annFile)

with open(OUTPUT_METRICS_FILE_PATH,"w+") as f:
  f.write("Id,gc-Precision,gc-Recall,gc-Mixed,ccd-Precision,ccd-Recall,ccd-Mixed,ccnd-Precision,ccnd-Recall,ccnd-Mixed\n")

  for i in range(4,len(sys.argv)-3,4):
    cocoImgId = int(sys.argv[i])
    label = cocoImgId
    if cocoImgId == 200000:
      cocoImgId = 44699

    correctedMaskPath = sys.argv[i+1]
    correctedNoDataMaskPath = sys.argv[i+2]
    gcMaskPath = sys.argv[i+3]


    M_gc=compareMaskWithCocoMask(cocoImgId,gcMaskPath)
    M_cd=compareMaskWithCocoMask(cocoImgId,correctedMaskPath)
    M_cnd=compareMaskWithCocoMask(cocoImgId,correctedNoDataMaskPath)

    f.write(
      "{},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f}\n".format(
          label,
          M_gc["precision"],M_gc["recall"],0.5*(M_gc["precision"]+M_gc["recall"]),
          M_cd["precision"],M_cd["recall"],0.5*(M_cd["precision"]+M_cd["recall"]),
          M_cnd["precision"],M_cnd["recall"],0.5*(M_cnd["precision"]+M_cnd["recall"])
      )
    )
