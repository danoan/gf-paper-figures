#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH="${SCRIPT_PATH%gf-paper-figures*}gf-paper-figures"

INPUT_FOLDER="${SCRIPT_PATH}/input"

source ${PROJECT_PATH}/plot-scripts/graph-plot.sh

function gp_plot_config()
{
	printf "set title '$1';
	set key right top;"


    # set yrange[1:];
	# set logscale x 10;
	# set xrange[0.1:1000];
	# set xtics ('0' 0.1,1,10,100,1000);
	# set xlabel 'Iterations';
	# set ylabel 'Elastica ({/Symbol a}=1/64, {/Symbol b}=1)';"
}

function gp_save()
{
    printf "
	set key right top;
    unset border;
    set polar;
    set angles degrees;
    
    set style line 10 lt 1 lc 0 lw 0.3;
    
    set grid polar 45; 
    set grid ls 10;
   
    unset xtics;
    unset ytics;
	
    set logscale r 2;
	
    set rtics axis;
    set rrange [0.5:32];
    set rtics scale 0;
    set rtics ('1' 1, '2' 2, '4' 4, '8' 8, '16' 16, '32' 32);
    set rtics font 'Helvetica, 12';
    
    set size square;
    
    set key lmargin;
    
    set_label(x, text) = sprintf(\"set label '%%s' at (6.5*cos(%%f)), (6.5*sin(%%f))     center\", text, x, x); 
    
    eval set_label(0, '3');
    eval set_label(45, '4');
    eval set_label(90, '5');
    eval set_label(135, '6');
    eval set_label(180, '7');
    eval set_label(225, '8');
    eval set_label(270, '9');
    eval set_label(315, '10');
    eval set_label(360, '3');
    
    
    set size 1.0, 0.6;

    set style line 1 lc rgb '#bf3a2c' pt 1 ps 0 lt 1 lw 2;
    set style line 2 lc rgb '#577bb4' pt 2 ps 0 lt 1 lw 2;
    set style line 3 lc rgb '#77ac50' pt 3 ps 0 lt 1 lw 2;
    set style line 4 lc rgb '#8551a0' pt 4 ps 0 lt 1 lw 2;
    set style line 5 lc rgb '#df8528' pt 5 ps 0 lt 1 lw 2;
    set style line 6 lc rgb '#fdfd4a' pt 6 ps 0 lt 1 lw 2;
	set style line 7 lc rgb '#915b32' pt 7 ps 0 lt 1 lw 2;

    set terminal postscript portrait enhanced color lw 2 \"Helvetica\" 20;
    set output \"${temp_plot}\";
    replot;
    "
}

