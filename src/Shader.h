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
#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC// needed for GLEW to be linked statically
#include <GL/glew.h>//needed for buffer creation and GLints -- also in Game and Mesh
class Shader
{
    public:
        Shader(void);
        Shader(GLuint shaderIndex);
        ~Shader(void);
        GLuint shaderProgram;
        GLint uniViewProjMatrix;
        GLint uniModelMatrix;
        GLint uniDirLight;//need to change this later

        //Vertex Array Attribute Pointers
        GLint uvAttrib;
        GLint colorAttrib;
        GLint normalAttrib;
        GLint posAttrib;
};
#endif
