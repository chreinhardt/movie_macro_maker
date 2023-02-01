#!/bin/sh
#
# movie_for_ravit_id Script
#
# Make a movie from a series of images.
#
# USAGE: bash movie_ravit.sh movie.avi
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

if [ $# != 1 ]
		then
		echo  "USAGE: bash movie_ravit.sh <movie.avi>" # Check the parameters
		exit 1;
fi

FILE="$1"

mogrify -format jpg *.png

mencoder "mf://*.jpg" -o "$FILE.avi" -ovc lavc -lavcopts vcodec=mjpeg

rm *.jpg

exit 0

