asmlib-opencv can be compiled by G++, VC on Linux, Windows(both VS and mingw) and Mac OS X.

If you have OpenCV 2.1 installed, simply install cmake, and then compile it with cmake.

# Source #
Source code is available in the SVN repository, please go to "source" tab and check out code.

# CMake #
If you are not familiar with CMake, simply run its GUI tool (in Linux it's called `cmake-gui`). The source directory is just the "`src`" you checked out. For building directory, I would suggest you creating a "build" directory inside "src".

After filling in the two directories, simply click "Configure". Then do what you do for general OpenCV projects.

Options:
  * **CMAKE\_BUILD\_TYPE**: You can try "Release", "Debug", "RelWithDebug"
  * **ENABLE\_ANNOTATOR**: asmlib-opencv comes with an simple annotator. If you have Qt installed, you can enable it.

Always click "Configure" after changing any options, and click "Generate" at last. A makefile/Visual Studio project will be generated, compile it accordingly.

# Run it #
Please check [RunningDemo](RunningDemo.md)