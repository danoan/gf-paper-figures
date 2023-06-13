from __init__ import *
import subprocess,sys,os,time

from param_combinator import *
from config import *
from template_render import *

GRAPH_FLOW_APP="set in read  input"
OUTPUT_FOLDER="set in read  input"
SUMMARY_FLOW_APP="set in read  input"

def resolve_output_folder(c):
    output_folder=OUTPUT_FOLDER
    for e in c:
        output_folder += "/" + e['path']

    return output_folder

def exhaustive_gc_flow(c):

    outputFolder = resolve_output_folder(c)
    shape,radius,opt_band,neigh_size,length_pen,gs = c

    s=" ".join( ["%s%s" % ("-S",shape['value']),
                 "%s%d" % ("-i",ITERATIONS),
                 "%s%d" % ("-r",radius['value']),
                 "%s%f" % ("-h", gs['value']),
                 "%s%f" % ("-a",length_pen['value']),
                 "%s%f" % ("-O",opt_band['value']),
                 "%s%d" % ("-n", NUM_THREADS),
                 "%s%s" % ("-N",neigh_size['value']),
                 "%s%d" % ("-B",30),
                 "%s" % ("-s",)

                 ])

    print("\n*****Running: ", s,"\n")

    binary = GRAPH_FLOW_APP
    subprocess.call( [binary,
                      "%s%s" % ("-S",shape['value']),
                      "%s%d" % ("-i",ITERATIONS),
                      "%s%d" % ("-r",radius['value']),
                      "%s%f" % ("-h", gs['value']),
                      "%s%f" % ("-a",length_pen['value']),
                      "%s%f" % ("-O",opt_band['value']),
                      "%s%d" % ("-n", NUM_THREADS),
                      "%s%s" % ("-N",neigh_size['value']),
                      "%s%d" % ("-B",30),
                      "%s" % ("-s",),
                      outputFolder
                      ] )

def summary_flow(c):
    binary = SUMMARY_FLOW_APP
    flow_images_folder_path=resolve_output_folder(c)

    shape,radius,opt_band,neigh_size,length_pen,gs = c

    opt_radius = 1.0/(length_pen['value']**0.5)

    jump=10
    subprocess.call( [binary,
                      flow_images_folder_path,
                      "%s/summary.svg" % (flow_images_folder_path,),
                      "%s%d" % ("-j",jump),
                      "%s%s" % ("-c","classic"),
                      "%s%f" % ("-r",opt_radius),
                      "%s%f" % ("-h",gs['value']),
                      "%s%s" % ("-e",".png")])

    subprocess.call( [binary,
                      flow_images_folder_path,
                      "%s/summary.eps" % (flow_images_folder_path,),
                      "%s%d" % ("-j",jump),
                      "%s%s" % ("-c","classic"),
                      "%s%f" % ("-r",opt_radius),
                      "%s%f" % ("-h",gs['value']),
                      "%s%s" % ("-e",".png")])


def read_input():
    if len(sys.argv)<3:
        print("Parameters missing! PROJECT_FOLDER RELATIVE_BUILD_FOLDER SUMMARY_FLOW_APP")
        exit(1)

    global OUTPUT_FOLDER, GRAPH_FLOW_APP,SUMMARY_FLOW_APP
    GRAPH_FLOW_APP=sys.argv[1]
    SUMMARY_FLOW_APP=sys.argv[2]
    OUTPUT_FOLDER=sys.argv[3]


def total_combinations():
    total=0
    combs = combinations(CONFIG_LIST)
    for c in combs:
        total+=1
    return total

def main():
    read_input()
    print("Total combinations: ",total_combinations())
    for c in combinations(CONFIG_LIST):
        if(valid_combination(c)):
            exhaustive_gc_flow(c)
            summary_flow(c)

    render_template("flow",CONFIG_LIST,OUTPUT_FOLDER)

if __name__=='__main__':
    main()
