# LWGLGF

The ideals behind the The LightWeight OpenGL Game Framework are to be as simple and straight-forward as possible, hopefully demystifying engine code to the beginning programmer. To do this, the LWGLGF prjoect strives to provide clean, documented and readable C/C++, minimize the use of external libraries so that one can see exactly what is going on and can change it at will, to be as cross-platform as possible, and to provide an sublime coding experience similar to XNA/MonoGame (with hints of functionality/syntax from engines like Unity).

To achieve those goals, open standards like OpenDDL and OpenGEX, PNG files, Ogg Vorbis, etc. will be preferred whenever
possible, to both draw from and hopefully give back to the open source community. In the long run - for the sake of standards and convenience - only a few image formats, audio formats, and geometry/mesh formats are intended to be supported in order to keep the framework lean but versatile.

## Project Dependencies:

This project utilizes [CMake](www.cmake.org) to generate and find the files needed to compile a LWGLGF-derivative game. Premake support is planned, but at this time unfinished.

Please B.Y.O.B. (Bring your own binaries)! The list of the libraries used is provided at the end of this ReadMe, but you'll need to create your own binaries/libraries for your OS/compiler pairing of choice. Eventually the project will utilize Git submodules (hopefully) to provide developers and contributors with the source code for the external libraries being used, but as of yet this is untested.

## Setting up the project for use:

Either fire up CMake or manually add the files to the IDE/makefile of your choosing. Feel free to modify the CMakes files/modules/define the associated variables to an alternative location if something doesn't look right/work properly.

## Creating your own game with LWGLGF:

Presently you have two options presently for writing your own game; remove the main() function from the top of Game.cpp and create a child class of Game [and add a main() method to whatever other file you wish], or just overwrite the code present in Game. Please note that presently there IS some demo code in the Game.cpp file, both to show how to access various functionality and for use in test-compiling the project to have a working executable.

## Credit and Licensing:

All credit for external libraries go to their respective rights/license holders, the list of and links to which are cited
below. Any helpful/useful code I've found online (tutorials, etc.) is cited in the .cpp or header of the class that uses that code.


The LWGLGF source code is to be freely distributed under the MIT License. Each source file has the license written at the top of the file.

## External Libraries Found at:

* [GLM - http://glm.g-truc.net](http://glm.g-truc.net)
* [GLEW - http://glew.sourceforge.net/](http://glew.sourceforge.net/)
* [GLFW - http://www.glfw.org/](http://www.glfw.org/)

## CMake modules provided courtesy of:

* [OpenSubdiv](https://github.com/PixarAnimationStudios/OpenSubdiv) 
  * Specifically `FindGLFW` and `FindGLEW`, found in `/cmake`.
* [GLM](https://github.com/g-truc/glm)
  * Specifcally `FindGLM`, found in  `/utils`.

Said files are found in the `/cmake` directory of LWGLGF and are subject to the terms of their own licenses/rights holders.
