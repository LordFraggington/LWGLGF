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
set(CMAKE_CXX_FLAGS "-o LWGLGF -std=c++11 -O0 -g3 -Wall -Wconversion -fmessage-length=0 -v -static -s")# no -c flag (doesn't link the libraries, just compiles)

#force the "install" path to be the build folder -- CMake ignores the destination presently
set(EXECUTABLE_OUTPUT_PATH ${LWGLGF_SOURCE_DIR}/build)

#find the OpenGL package/directory - the rest of the includes/libraries we use should be using must be added to the corresponding directory 
find_package(OpenGL REQUIRED) #in windows-land it will complain, but you can't get it to compile an OpenGL Application otherwise

#include the appropriate directories
include_directories(${LWGLGF_SOURCE_DIR}/includes ${OPENGL_INCLUDE_DIRS})
link_directories(${LWGLGF_SOURCE_DIR}/libraries ${OPENGL_LIBRARIES})

#detect OS/compiler?; Remember the difference between MSVC compiled versus MinGW compiled libraries, etc. Still have a dependency issue in MinGW with libstdc++-6.dll and the like ( -static-libgcc -static-libstdc++ don't work)

#create executable -- remember to list all of the files that make up the final "product"
add_executable(LWGLGF Game Camera Vertex Mesh GameObject GameEntity Light Material Shader ResourceManager)

#link! those! libraries! (the .dll's/.so's/dylib's need to be present at runtime, but that's another can of worms -- hence our attempts to statically link them!)
target_link_libraries(LWGLGF glfw3 glew32 opengl32 liblua.a  libOpenAL32)#glfw3dll only used if using the dll (doi); glew32mxs glew32mx are only for multiple rendering contexts; inexplicably, liblua needs .a to be found?

#create "build" directory and dump all of our cool toys there -- hacking it for now (see above)
install(TARGETS LWGLGF RUNTIME DESTINATION ${LWGLGF_SOURCE_DIR}/build)

#copy over any objs, shaders, and lua scripts, etc. into a /build subdirectory
#install(DIRECTORY ${LWGLGF_SOURCE_DIR}/assets DESTINATION ${LWGLGF_SOURCE_DIR}/build) --necessary?
file(COPY ${LWGLGF_SOURCE_DIR}/assets DESTINATION ${LWGLGF_SOURCE_DIR}/build)
#install(DIRECTORY ${LWGLGF_SOURCE_DIR}/shaders DESTINATION ${LWGLGF_SOURCE_DIR}/build) -- necessary?
file(COPY ${LWGLGF_SOURCE_DIR}/shaders DESTINATION ${LWGLGF_SOURCE_DIR}/build)