# Copyright 2014 - 2016, Clayton Andrews.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required(VERSION 2.8)

#Define the project name (and type, if desired)
project(LWGLGF CXX)

#Set the project version number
set(LWGLGF_VERSION_MAJOR 0)
set(LWGLGF_VERSION_MINOR 1)
set(LWGLGF_VERSION_PATCH 0)

#toggle debug/release mode; false for release
set(DEBUG TRUE)

#force the "install" path to be the build folder -- CMake ignores the destination presently
set(EXECUTABLE_OUTPUT_PATH ${LWGLGF_SOURCE_DIR}/build)

#Set file path for custom Find*.cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LWGLGF_SOURCE_DIR}/cmake/")

#Add the external submodules so Cmake can traverse to the appropriate directories to build them
add_subdirectory(${LWGLGF_SOURCE_DIR}/extern/glfw)
add_subdirectory(${LWGLGF_SOURCE_DIR}/extern/glm)
add_subdirectory(${LWGLGF_SOURCE_DIR}/extern/glew/build/cmake)

#Configure the variables necessary to find_package(XYZ)
set(GLFW_LOCATION ${LWGLGF_SOURCE_DIR}/extern/glfw)
set(GLEW_LOCATION ${LWGLGF_SOURCE_DIR}/extern/glew)
set(GLM_ROOT_DIR ${LWGLGF_SOURCE_DIR}/extern/glm)

#find the packages/directories we use should be using; alternatively, define the include/library directory variables here
find_package(OpenGL REQUIRED) #in Windows-land Cmake will complain (but still find/link the package), but you can't get it to compile an application utilizing OpenGL otherwise
#find_package(GLFW REQUIRED)
#find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)#find file needs to work on "not Linux" file systems; currently have it looking where it is installed if you call mingw32-make install, may need to generalize

#include the appropriate directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

#create executable -- remember to list all of the files that make up the final "product"
add_executable(LWGLGF src/Game src/Camera src/Vertex src/Mesh src/GameObject src/Transform src/Light src/Material src/Shader src/ResourceManager)

#Set compiler flags based on system and debug/release status
#These flags currently work/have been tested with MinGW(32 and 64)/GCC
if(WIN32)
	if(MSVC AND (NOT (MSVC_VERSION LESS 1800)))#require MSVC12 (Currently the most C++11 compliant, and the current goal is to utilize said features when possible)
		if(DEBUG)
			set(CMAKE_CXX_FLAGS "/FeLWGLGF /Wall /Za")
      target_link_libraries(LWGLGF ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})
			#message("sample WIN32 debug mode message for compliant compilers")
		else(DEBUG)
			#set release flags here
		endif(DEBUG)
  else(MSVC AND (NOT (MSVC_VERSION LESS 1800)))
		#currently assuming we are working with MinGW 32/64 currently; detection for other compilers should be added here later
		if(DEBUG)
			set(CMAKE_CXX_FLAGS "-o LWGLGF -std=c++11 -O0 -g3 -Wall -Wconversion -fmessage-length=0 -v -s")
      target_link_libraries(LWGLGF ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})
		else(DEBUG)
			#set release flags here
		endif(DEBUG)
  endif(MSVC AND (NOT (MSVC_VERSION LESS 1800)))
else(WIN32)
	#No Apple specific code yet, so this is currently targeting GNU/Linux and MinGW/GCC
	if(DEBUG)
		set(CMAKE_CXX_FLAGS "-o LWGLGF -std=c++11 -O0 -g3 -Wall -Wconversion -fmessage-length=0 -v -s")
    #No Apple code yet; these Linux libraries were all that was needed on Crunchbang and Ubuntu (when testing)
    target_link_libraries(LWGLGF GL X11 Xrandr glfw3 GLEW)
	else(DEBUG)
		#set release flags here
	endif(DEBUG)
endif(WIN32)

#create "build" directory and dump all of our cool toys there
install(TARGETS LWGLGF RUNTIME DESTINATION build)
#copy over any objs, shaders, etc. into the /build subdirectory
#The .dll's/.so's/dylib's need to be present at runtime, but that's another can of worms; at some point there the pros and cons of statically linking libraries should be evaluated
install(DIRECTORY assets DESTINATION build)
install(DIRECTORY shaders DESTINATION build)

#Notes:
#Remember the difference between MSVC compiled versus MinGW compiled libraries, etc.
#Still have a dependency issue in MinGW with libstdc++-6.dll and the like ( -static-libgcc -static-libstdc++ don't work), was using -static with MinGW
#glfw3dll only used if using the dll; glew32mxs and glew32mx are only for multiple rendering contexts.
# no -c flag (doesn't link the libraries, just compiles)
# MSVC compiler options found here:https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx
