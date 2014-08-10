ssim
====

Program to measure the similarity between two videos using the OpenCV library and the structural similarity algorithm (SSIM). This is modified from the video-input-psnr-ssim tutorial of OpenCV. This program has been tested to work on the Windows platform only.


##Tested Dependencies
1. OpenCV (2.4.9)
2. Cmake (3.0.1)
3. Mingw, GCC 4.8.1


###Installing Dependencies
1. Download and install [Mingw](http://sourceforge.net/projects/mingw/files/). Check the options, "mingw32-base" and "mingw32-gcc-g++". Add "C:\MinGW\bin" to your PATH.
2. Download and install [Cmake for Windows](http://www.cmake.org/cmake/resources/software.html). Choose to add to system path during installation.
3. Download OpenCV 2.4.9 [here](https://github.com/Itseez/opencv/archive/2.4.9.zip) from the github releases. Do not use the official Windows binary unless you wish to use Visual Studio to compile your project. Unzip the zip file to `C:\OpenCV`.
4. Open CMake GUI. Set source to `C:\opencv` and where-to-build to `C:\opencv\build\x86\mingw`. Click "Configure" then "Generate"
5. Compile OpenCV using Mingw
```bash
cd C:\opencv\build\x86\mingw
#Set N in -j to the number of CPU cores you have for parallel compilation. For example -j4.
mingw32-make -jN
mingw32-make install
#Do not run "mingw32-make clean"
```
6. Add `C:\opencv\build\x86\mingw\bin` to your PATH

