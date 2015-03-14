After [compiling](http://code.google.com/p/asmlib-opencv/wiki/HowtoCompile) the project, a demo will be ready in BUILDPATH/demo/demo

If you're using Windows, a binary demo is already in [Downloads](http://code.google.com/p/asmlib-opencv/downloads/list) page.

# Options #
```
Arguments: 
===========
Tasks: 
   -b : Build a model!
   -v : View a model.
   -f : Fit images to a model.

General options:
   -m <path_to_model> : Path to the model file.
   -V <number>        : Verbose level.

Build specific options:
   -d <def_file>  : Model definition file, see wiki.
   -l <list_file> : List of labelled points, see wiki.

Fitting specific options: 
   -C <detector_xml> : Face/Object detector XML (OpenCV).
   -p <image>        : Path to an image
   -pc               : Run on your webcam!
```

# Examples #
## View a Model ##
You can view models. Some models are available in the SVN repository:
```
./demo -m ../../../data/muct76.model -v
```

You can choose a parameter and adjust it.

## Fit to an Image ##
Fit the model to your webcam stream:
```
./demo -pc -m ../../../data/muct76.model -f -C ../../../data/haarcascade_frontalface_alt.xml
```

or
```
./demo -m ../../../data/muct76.model -f -C ../../../data/haarcascade_frontalface_alt.xml -p <PATH_TO_YOUR_IMAGE>
```

## Build a Model ##
Please check page [BuildModel](BuildModel.md)