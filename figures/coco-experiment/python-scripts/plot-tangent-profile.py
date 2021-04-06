import sys
import matplotlib.pyplot as plt
import pandas as pd

def plot_tangent_profile(datafilepath,outputfilepath):
  data = pd.read_csv(datafilepath)
  fig,ax = plt.subplots()

  ax.plot( data["n"],data["tangent"] )
  ax.set_xlabel("linel number")
  ax.set_ylabel("angle")
  fig.savefig(outputfilepath,
  bbox_inches='tight',
  pad_inches=0)

def plot_metrics(datafilepath,outputfilepath,title):
  data = pd.read_csv(datafilepath)

  data["inflection"] /= data["inflection"][0]
  data["elastica"] /= data["elastica"][0]

  plt.style.use("seaborn-whitegrid")
  fig,ax = plt.subplots()

  ax.plot( data["iteration"],data["inflection"] )
  ax.plot( data["iteration"],data["elastica"] )
  ax.plot( data["iteration"],data["area-similarity"] )

  ax.set_xlim(0,10)
  ax.set_xlabel("iterations")
  ax.set_ylabel("ratio from initial value")
  ax.set_title(title)

  ax.legend( ["inflection points","elastica","area-similarity"] )

  fig.savefig(outputfilepath)

TANGENT_PROFILE_DATA=sys.argv[1]
OUTPUT_PATH=sys.argv[2]

plot_tangent_profile(
  "{}".format(TANGENT_PROFILE_DATA),
  "{}".format(OUTPUT_PATH)
)
