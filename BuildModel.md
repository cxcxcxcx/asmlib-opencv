# Introduction #
As landmark datasets are available online, asmlib can build models based on those tools.

# From MUCT database #
There are some scripts to help you build an ASM model from MUCT database.

If you have Linux installed, it will be VERY easy. Just check `data/muct/README.txt` .

If you want to start from scratch, see the instructions below.


# Build Training Set from Scratch #
If you have your own landmark labelling set, or if you're working something other than face, this will hopefully help.

## Training Files ##
The training data contains three types of files:
  * Model definition file: number of mark points, etc
  * PTS file: a PTS file lists the coordinates of landmark points in an image
  * List file: A list of paths of PTS files

## Model Definition ##
Below is an example. The order of this file should never be changed. Lines starting with a # are treated as comments.
```
#Number of landmark points
60
# number of paths
10
# Face Contour:
# Formats: to-point-# path-closed
# 
# A path from the 1st point to the 15th point, this path is not a cycle:
15 0
# Another path from 16th point to the 21st one, this path is a closed cycle.
21 1
27 1
31 1
32 0
36 1
37 0
46 1
48 0
60 1

# Initial positions
# After face detection, we may want to move the box of
# face to get better results. Eg. If the model contains 
# only points below the eye, we want to put the initial 
# estimation in the lower part of a face.
#r.y -= r.height*?
0.1
#r.x -= r.width*?
0.1
#r.width *= ?
1.2
#r.height *= ?
1.45

# More explanations on the following will be given later
#step?*100/sqrt(area)
1.0

# init scale ratio when searching
0.65
# init X offset when searching
0
# init Y offset when searching
0.25
```

## PTS file ##
A PTS file lists the coordinates of landmark points in an image. The path of the image is indicated by the path of the PTS file. eg: File 'feret\_color/00002\_940928\_fa.ppm.pts75' is describing the landmark points in 'feret\_color/00002\_940928\_fa.ppm'.

The first line of a PTS file has an integer N, indicating how many markpoints there are. Each of the following N lines contains two integers X\_i and Y\_i, describing the coordinates of the points (they are in OpenCV coordinates, and must be integers).

E.g. a description of four points:
```
4
381 305
354 267
314 241
268 230
```

# List File #
A list of PTS files. Every line represents a path to a PTS file. Either absolute path or relative path is OK. eg:
```
/mnt/data/asmlib-opencv/data/feret_color/00002_931230_fa.ppm.pts75
/mnt/data/asmlib-opencv/data/feret_color/00002_940128_fa.ppm.pts75
/mnt/data/asmlib-opencv/data/feret_color/00002_940422_fa.ppm.pts75
```

or in the case above, you can put a file in `/mnt/data/asmlib-opencv/data/my.list`, containing:
```
feret_color/00002_931230_fa.ppm.pts75
feret_color/00002_940128_fa.ppm.pts75
feret_color/00002_940422_fa.ppm.pts75
```