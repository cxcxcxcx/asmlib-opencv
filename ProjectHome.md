![http://asmlib-opencv.googlecode.com/svn/wiki/me.jpg](http://asmlib-opencv.googlecode.com/svn/wiki/me.jpg)

An open source Active Shape Model library written by C++ using OpenCV 2.0 (or above), no other dependencies. Thanks to CMake, the library has been successfully compiled in following environments:
  * Linux (both 32 and 64 bits)
  * Windows(both VC and MinGW)
  * Mac OS X
  * Android
Both training and fitting codes are provided. For Windows users, a binary demo is available for [download](http://code.google.com/p/asmlib-opencv/downloads/list).

The library implements ASM and BTSM(Bayesian Tangent Shape Model). I think its result is good for most frontal faces. The fitting speed is fast, on my laptop computer(Intel(R) Core(TM)2 Duo CPU P8600 @ 2.40GHz), it can do real-time fitting on a webcam (see [RunningDemo](RunningDemo.md)).


With the code provided, you can also train your own model all by yourself (see [BuildModel](BuildModel.md)). For face model, scripts to train a model from the [MUCT Dataset](http://www.milbo.org/muct/) are provided.

**Codes are in the SVN repository, click "Source" to check them out. Documentation for the codes is available [here](http://chenxing.name/docs/asmlib/).**

If you have questions, please either file a issue or contact me directly (cxcxcxcx, gmail).

If you use the library in your project, please just add a link to this project page, thanks! But if you want to use it in proprietary applications, please contact me first (currently it's published under LGPL v3).