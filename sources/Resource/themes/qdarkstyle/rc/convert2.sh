#!/bin/bash
mkdir converted
find *.png | while read in;
 do name=$(echo $in | cut -d "." -f 1); 
name1=$(echo $name |tail -c 2)
name2=$(echo $name | cut -c1-1)


./colorbalance -c green -a 25  "$in" "./converted/$in"
echo $name;
done;

#-r midtones

# USAGE: colorbalance [-c color] [-a amount] [-r region] [-m mid] [-f factor] [-l low] [-h high] [-t taper] infile outfile
#USAGE: colorbalance [-help]

#-c .... color ..... color to modify: red (r), yellow (y), green (g),
#................... cyan (c), blue (b), magenta (m); default=red
#-a .... amount .... amount of color change; 0<=float<=100; default=10
#-r .... region .... region to change: midtones (m), shadows (s),
#................... highlights (h), all (a); default=all
#-m .... mid ....... mid value threshold; 0<=float<=100; default=mean
#-l .... low ....... low value threshold; 0<=float<=100;
#................... default=mean-factor*std
#-h .... high ...... high value threshold; 0<=float<=100;
#................... default=mean+factor*std
#-f .... factor .... standard deviation factor; float>0; default=0.5
#-t .... taper ..... taper for range thresholds in pixels; integer>=0;
#................... default=10

#PURPOSE: To manually color balance an image in midtones, highlights, shadows or overall.

#DESCRIPTION: COLORBALANCE manually color balances an image according to a user selected color, region and amount. The regions are midtones, shadows, highlights or all (i.e. the whole image). The regions thresholds default to mean +/- factor*standard-deviation of the appropriate channel or can be set manually. The region boundaries can be tapered so that they change gradually.

#ARGUMENTS:

#-c color ... COLOR to modify. The choices are: red (r), yellow (y), green (g), cyan (c), blue (b) or magenta (m). The default=red

#-a amount ... AMOUNT of color change. Values are in the range 0<=float<=100. The default=10.

#-r region ... REGION of image to change. The choices are: midtones (m), shadows (s), highlights (h) or all (a) for the whole image. The default=all.

#-m mid ... MID is the threshold value that determines the center of the midtone range. Values are in the range 0<=float<=100. The default is the appropriate channel mean.

#-l low ... LOW is the threshold value that determines the low end of the midtones and the high end of the shadows. Values are in the range 0<=float<=100. The default is the appropriate channel mean - factor*standard-deviation

#-h high ... HIGH is the threshold value that determines the high end of the midtones and the low end of the highlights. Values are in the range 0<=float<=100. The default is the appropriate channel mean + factor*standard-deviation

#-f factor ... FACTOR is the multiplication factor for the standard deviation used for the low and high threshold values. Values are floats>0. The default=0.5.

#-t taper ... TAPER is the taper for range the thresholds in pixels. Values are integers>=0. The default=10

#CAVEAT: No guarantee that this script will work on all platforms, nor that trapping of inconsistent parameters is complete and foolproof. Use At Your Own Risk. 


