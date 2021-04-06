import sys
import matplotlib.pyplot as plt
import pandas as pd


def compareLabels(data,label1,label2,column):
  df_greater = data["{}-{}".format(label1,column)] > data["{}-{}".format(label2,column)]
  return df_greater[ df_greater==True ].count() / df_greater.count()

def compareResults(data,label1,label2):
  return "(Precision) {} better than {}: {:.4f}\n\
  (Recall) {} better than {}: {:.4f}\n\
  (Mixed) {} better than {}: {:.4f}\n".format(
    label1,label2,compareLabels(data,label1,label2,"Precision"),
    label1,label2,compareLabels(data,label1,label2,"Recall"),
    label1,label2,compareLabels(data,label1,label2,"Mixed")
    )

def scatter_plot(data,metric="Precision"):
  xlabels=[0]
  xlabels.extend( list(data["Id"]) )

  nImgs = len(data)
  xlabelPos = list(range(1,nImgs+1))

  fig,ax = plt.subplots(figsize=(20,10))
  fig.suptitle("{} for different methods".format(metric))

  ax.scatter(x=xlabelPos,y=data["gc-{}".format(metric)])
  ax.scatter(x=xlabelPos,y=data["ccd-{}".format(metric)])
  ax.scatter(x=xlabelPos,y=data["ccnd-{}".format(metric)])

  xticks = [0]
  xticks.extend(xlabelPos)
  ax.set_xticks(xticks)
  ax.set_xticklabels(xlabels,rotation=90)
  ax.legend( ["Graph Cut","Contour Correction-With-Data","Contour Correction Without Data"] )

  fig.savefig("{}/{}.png".format(OUTPUT_DIR,metric),
  bbox_inches='tight',
  pad_inches=0)



plt.style.use("seaborn-whitegrid")

PRECISION_RECALL_METRICS=sys.argv[1]
CORRECTION_METRICS=sys.argv[2]
OUTPUT_DIR=sys.argv[3]

data_recall = pd.read_csv(PRECISION_RECALL_METRICS)
data_correction = pd.read_csv(CORRECTION_METRICS)

scatter_plot(data_recall,metric="Precision")
scatter_plot(data_recall,metric="Recall")
scatter_plot(data_recall,metric="Mixed")

with open("{}/results.txt".format(OUTPUT_DIR),"w") as f:
  f.write(compareResults(data_recall,"ccd","ccnd"))
  f.write(compareResults(data_recall,"ccd","gc"))


#Box plot
plt.rcParams.update({'font.size': 22})

fig,ax = plt.subplots(figsize=(20,10))
fig.suptitle("GF normalization metrics with respect to grabcut")
data_correction["Elastica\nreduction"] = (data_correction["elastica-gc"] - data_correction["elastica-cc"])/data_correction["elastica-gc"]
data_correction["Inflection\nreduction"] = (data_correction["inflection-gc"] - data_correction["inflection-cc"])/data_correction["inflection-gc"]

data_correction[ ["Elastica\nreduction","Inflection\nreduction"]].plot.box(ax=ax)
fig.savefig("{}/box-plot-correction.png".format(OUTPUT_DIR),
bbox_inches='tight',
pad_inches=0)

fig,ax = plt.subplots(figsize=(20,10))
fig.suptitle("0.5 x (Precision + Recall)")
temp=data_recall.rename({"gc-Mixed":"grabcut","ccd-Mixed":"GF\nwith data", "ccnd-Mixed": "GF\nwithout data"},axis=1)
temp[ ["grabcut","GF\nwith data","GF\nwithout data"] ].plot.box(ax=ax)
fig.savefig("{}/box-plot-mixed.png".format(OUTPUT_DIR),
bbox_inches='tight',
pad_inches=0)
