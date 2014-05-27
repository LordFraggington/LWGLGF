#version 330
/*
 * Copyright 2014, Clayton Andrews.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//Everything necessary to render from the engine
uniform mat4 ViewProjMatrix;
uniform mat4 ModelMatrix;

//Everything needed/contained in the Vertex struct
in vec2 uv;
in vec4 color;
in vec3 vPosition;
in vec3 normal;

out vec2 UV;
out vec4 Color;
out	vec3 Normal;

void main()
{	
	//pass through to the fragment shader
	Color = color;
	UV = uv;
	Normal = normal;

	//assign the position of the current vertex in world space (from local space)
	gl_Position = ViewProjMatrix * ModelMatrix * vec4(vPosition, 1);
}