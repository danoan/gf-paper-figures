# Put your gnuplot commands here. Everything except
# the last 'plot ....' line.

set key right top;
set xrange [10:1];
set yrange [MIN_Y:MAX_Y];
set xlabel 'Height';
set ylabel TITLE;

set style line 1 lc rgb '#bf3a2c' pt 1 ps 2 lt 1 lw 2 pi 2;
set style line 2 lc rgb '#577bb4' pt 2 ps 2 lt 1 lw 2 pi 2;
set style line 3 lc rgb '#77ac50' pt 3 ps 2 lt 1 lw 2 pi 2;


set style line 11 lc rgb '#808080' lt 1;
set border 3 back ls 11;
set tics nomirror;

set style line 12 lc rgb '#808080' lt 0 lw 1;
set grid back ls 12;


# Also set these parameters (but don't delete any line!)
# ______________________________________________________
video_file="animation"
video_file_suffix=".avi"

start_frame=START_FRAME
end_frame=END_FRAME
skip_frame=1

video_width =800
video_height=HEIGHT
font_size=15
font_path=\
"/usr/share/fonts/liberation/LiberationSans-Regular.ttf"
# ______________________________________________________

