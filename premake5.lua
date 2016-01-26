-- Copyright 2014 - 2016, Clayton Andrews.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.

-- First attempt at setting up a premake script for LWGLGF; this file is not finished and is not ready for use
workspace "LWGLGF"
  language C++
  configurations {"Debug", "Release"}
  --specify the possible build targets
  platforms { "windows", "linux", "mac" }
  --if a command isn't passed as an --os argument, Premake IDs and uses the host OS
  filter "platforms:windows"
    system "windows"
  filter "platforms:linux"
    system "linux"
  filter "platforms:mac"
    system "macosx"

project "LWGLGF"
  files { "src/*.cpp", "src/*.h"}
  includedirs {"extern/glew", "extern/glm", "extern/glfw/include/GLFW"}
  kind "WindowedApp"
  filter "system:windows"
    links{ "glew32", "glfw3", "opengl32"}
  filter "system:linux"
    links{"GL", "X11", "Xrandr", "glfw3", "GLEW"}
  filter "system:macosx"
    --Mac linkage to be inserted here
      links{ }
