ssim
====

Program to measure the similarity between two videos using the OpenCV library and the structural similarity algorithm (SSIM). This is modified from the [video-input-psnr-ssim tutorial](http://docs.opencv.org/doc/tutorials/highgui/video-input-psnr-ssim/video-input-psnr-ssim.html) of OpenCV. This program has been tested to work on the Windows platform only.

##Differences from video-input-psnr-ssim tutorial
1. Calculate SSIM for each and every frame.
2. Show overall progress every 50 frames.
3. Start comparison at different points of both videos.
4. Optional number of frames to compare. Compares up to the shortest video by default.

##Usage
```bash
#ssim reference_video_file test_video_file reference_start_frame test_start_frame [numFramesToCompare]
ssim reference.avi test.avi 0 1
ssim reference.avi test.avi 5 13 1000
```


##Screenshots

![Screen](/misc/working.png)
Preview windows are shown to indicate progress.

![Screen](/misc/results.png)
A percentage result is given at the end.

##Compiling this program
```bash
cmake -G "MinGW Makefiles" #First time only
mingw32-make
```

##Dependencies I used
1. OpenCV (2.4.9)
2. Cmake (3.0.1)
3. Mingw, GCC 4.8.1

###Installing Dependencies
1. Download and install [Mingw](http://sourceforge.net/projects/mingw/files/). Check the options, "mingw32-base" and "mingw32-gcc-g++". Add "C:\MinGW\bin" to your PATH.

2. Download and install [Cmake for Windows](http://www.cmake.org/cmake/resources/software.html). Choose to add to system path during installation.

3. Download OpenCV 2.4.9 [here](https://github.com/Itseez/opencv/archive/2.4.9.zip) from the github releases. Do not use the official Windows binary unless you wish to use Visual Studio to compile your project. Unzip the zip file to `C:\opencv`.

4. Open CMake GUI. Set source to `C:\opencv` and where-to-build to `C:\opencv\build\x86\mingw`. Click "Configure", select "MingGW Makefiles" and "Default native compilers" then "Generate"

5. Compile OpenCV using Mingw based on the commands below then add `C:\opencv\build\x86\mingw\bin` to your PATH.

```bash
cd C:\opencv\build\x86\mingw

#Change N in -j to the number of CPU cores you have for parallel compilation. For eg, -j4.
mingw32-make -jN
mingw32-make install
#Do not run "mingw32-make clean"
```

##References
1. [OpenCV install with Codeblocks and Mingw](http://kevinhughes.ca/tutorials/opencv-install-on-windows-with-codeblocks-and-mingw/)
2. [Video Input with OpenCV and similarity measurement](http://docs.opencv.org/doc/tutorials/highgui/video-input-psnr-ssim/video-input-psnr-ssim.html)

