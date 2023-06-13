#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH="${SCRIPT_PATH%gf-paper-figures*}gf-paper-figures"

INPUT_FOLDER="${SCRIPT_PATH}/input"

source ${PROJECT_PATH}/plot-scripts/graph-plot.sh

function gp_plot_config()
{
	printf "set title '$1';
	set key left bottom;
    set size 1.0, 0.6;
    
    set style line 10 lt 1 lc 0 lw 0.3;
    set grid ls 10;
    
    set logscale y 2;
	
    set ytics axis;
    set yrange [0.5:32];
    set ytics ('1' 1, '2' 2, '4' 4, '8' 8, '16' 16, '32' 32);
    
    set xtics axis;
    set xrange [3:10];
	
    set xlabel 'Estimation radius';
	set ylabel 'Hausdorff Distance';
    "
}

function gp_save()
{
    printf "
    set style line 1 lc rgb '#bf3a2c' pt 1 ps 1 lt 1 lw 2;
    set style line 2 lc rgb '#577bb4' pt 2 ps 1 lt 1 lw 2;
    set style line 3 lc rgb '#77ac50' pt 3 ps 1 lt 1 lw 2;
    set style line 4 lc rgb '#8551a0' pt 4 ps 1 lt 1 lw 2;
    set style line 5 lc rgb '#df8528' pt 5 ps 1 lt 1 lw 2;
    set style line 6 lc rgb '#fdfd4a' pt 6 ps 1 lt 1 lw 2;
	set style line 7 lc rgb '#915b32' pt 7 ps 1 lt 1 lw 2;
    
    set style line 11 lc rgb '#808080' lt 1;
    set border 3 back ls 11;
    set tics nomirror;

    set style line 12 lc rgb '#808080' lt 0 lw 1;
    set grid back ls 12;

    set terminal postscript portrait enhanced color lw 2 \"Helvetica\" 20;
    set output \"${temp_plot}\";
    replot;
    "
}

