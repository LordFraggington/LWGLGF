# Copyright 2014, Clayton Andrews.
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

#set compiler flags
if(WIN32)
set(CMAKE_CXX_FLAGS "-o LWGLGF -std=c++11 -O0 -g3 -Wall -Wconversion -fmessage-length=0 -v -static -s")
else(WIN32)
#winging it on crunchbang/ubuntu -- cannot statically link libGL or libopenal (also, no Apple-specific instructions yet)
set(CMAKE_CXX_FLAGS "-o LWGLGF -std=c++11 -O0 -g3 -Wall -Wconversion -fmessage-length=0 -v -s")
endif(WIN32)

#find the packages/directories we use should be using; alternatively, define the include/library directory variables here 
find_package(OpenGL REQUIRED) #in Windows-land Cmake will complain (but still find/link the package), but you can't get it to compile an application utilizing OpenGL otherwise
find_package(OpenAL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)#find file needs to work on "not Linux" file systems

#include the appropriate directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIR})
link_directories(${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENAL_LIBRARIES})

#create executable -- remember to list all of the files that make up the final "product"
add_executable(LWGLGF Game Camera Vertex Mesh GameObject GameEntity Light Material Shader ResourceManager)

#link! those! libraries! (the .dll's/.so's/dylib's need to be present at runtime, but that's another can of worms -- hence our attempts to statically link them!)
if(WIN32)
target_link_libraries(LWGLGF glfw3 glew32 opengl32 libOpenAL32)
else(WIN32)
#winging it on crunchbang/ubuntu (No Apple code yet)
target_link_libraries(LWGLGF GL X11 Xrandr glfw3 GLEW openal)
ENDIF(WIN32)

#force the "install" path to be the build folder -- CMake ignores the destination presently
#set(EXECUTABLE_OUTPUT_PATH ${LWGLGF_SOURCE_DIR}/build)

#create "build" directory and dump all of our cool toys there
install(TARGETS LWGLGF RUNTIME DESTINATION ${LWGLGF_SOURCE_DIR}/build)
#copy over any objs, shaders, etc. into the /build subdirectory
install(DIRECTORY ${LWGLGF_SOURCE_DIR}/assets DESTINATION ${LWGLGF_SOURCE_DIR}/build)
install(DIRECTORY ${LWGLGF_SOURCE_DIR}/shaders DESTINATION ${LWGLGF_SOURCE_DIR}/build)

#Notes:
#Remember the difference between MSVC compiled versus MinGW compiled libraries, etc.
#Still have a dependency issue in MinGW with libstdc++-6.dll and the like ( -static-libgcc -static-libstdc++ don't work)
#glfw3dll only used if using the dll; glew32mxs and glew32mx are only for multiple rendering contexts.
# no -c flag (doesn't link the libraries, just compiles)
