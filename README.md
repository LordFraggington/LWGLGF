The LWGLGF - LightWeight OpenGL Game Framework - started out as student rendering project in OpenGL for a homework
assignment (in fact, technically before that as an assignment for DirectX which was then translated to OpenGL), and
slowly grew (partially due to yet another class, plus some on the side tinkering) from there into what it is today.

The ideals behind the LWGLGF are to be simple and straight-forward. This is achieved by striving to provide clean,
readable C++ that is accessible to beginners and veterans alike, minimalistic use of external libraries,
striving to be as cross-platform as possible, and providing an environment similar to XNA/MonoGame (with hints of
functionality/syntax from engines like Unity) for a sublime coding experience where everything works out of the box.

To achieve those goals, open standards like OpenDDL and OpenGEX, PNG files, Ogg Vorbis, etc. will be preferred whenever
possible. In the long run - for the sake of standards and convenience - only a few image formats, audio formats, and geometry/mesh
formats are intended to be supported in order to keep the framework lean but versatile. 


Demo Application Controls (currently found in Game.cpp):

	W - Move Camera Forward
	A - Move Camera Left
	S - Move Camera Backward
	D - Move Camera Right
	L_SHIFT - Move Camera Up
	L_CONTROL - Move Camera Down
	Escape - Quit
	Up Arrow - Rotate Camera Up
	Down Arrow - Rotate Camera Down
	Left Arrow - Rotate Camera Left
	Right Arrow - Rotate Camera Right


Project Dependencies:

Please B.Y.O.B. (Bring your own binaries) - The list of the libraries used is provided at the end of this ReadMe, but you'll need to create your own binaries/libraries for your OS/compiler pairing of choice. LWGLGF "should" statically link all of the .a/.lib files you need, creating a "fire and forget" self-reliant executable.

With that said, currently (hopefully) the only external dependency for the application (on Windows at least) SHOULD the .dll for OpenAL (personally, I am currently having issues getting two MinGW libraries to statically link on my machine so that the only thing needed on a "clean" machine is OpenAL in some form). Using GNU/Linux, there are still some issues statically linking libraries due to some libraries not existing/not being readily available.


Setting up the project for use:

Either fire up CMake or manually add the files to the IDE/makefile of your choosing. Feel free to modify the CMakes files/modules/define the associated variables to an alternative location if something doesn't look right/work properly.


Creating your own game with LWGLGF:

Presently you have two options presently for writing your own game; remove the main() function from the top of Game.cpp and create a child class of Game [and add a main() method to whatever other file you wish], or just overwrite the code present in Game.


Credit and Licensing:

All credit for external libraries go to their respective rights/license holders, the list of and links to which are cited
below. Any helpful/useful code I've found online (tutorials, etc.) is cited in the .cpp or header of the class that uses that code.


The LWGLGF source code is to be freely distributed under the MIT License.


External Libraries Found at:

GLM (http://glm.g-truc.net/0.9.5/index.html)

GLEW (http://glew.sourceforge.net/)

GLFW (http://www.glfw.org/)

OpenAL Soft (http://kcat.strangesoft.net/openal.html)


CMake modules (found in LWGLGF/cmake) provided courtesy of:

OpenSubdiv (https://github.com/PixarAnimationStudios/OpenSubdiv)- FindGLFW and FindGLEW (found in /cmake)

GLM (http://glm.g-truc.net/0.9.5/index.html) - FindGLM (found in /utils)

Said files are subject to the terms of their own licenses/rights holders.
