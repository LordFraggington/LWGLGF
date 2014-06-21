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
#pragma once
#ifndef MESH_H
#define MESH_H
#include <stdlib.h>//wcstombs, EXIT_SUCCESS/FAILURE...etc.
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>//needed for buffer creation and GLints -- also found in Game.cpp
#include "Vertex.h"
class Mesh
{
	public:
		Mesh(wchar_t* filename);
		Mesh(void);
		~Mesh(void);
		
		/* The actual OpenGL Vertex Array Object and it's bound buffers */
		GLuint vertexObject;
		GLuint vertexBuffer;
		GLuint indexBuffer;

		/* used for construction primarily, but nice for debugging purposes/showing off (pending removal) */
		int indexCount;
		//int vertCount;
		//int uvCount;
		//int normalCount;

	private:
		Vertex* vertexArray; //saved for collision volume/object generation 
		GLuint* indexSequence; //really don't know if I need to keep this after creating the buffer
};
#endif
