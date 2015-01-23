/*
 * Copyright 2014 - 2015, Clayton Andrews.
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

#include "Shader.h"

Shader::~Shader(void)
{
  glDeleteProgram(shaderProgram);
}

/* Specify the layout of the vertex data for passing to the shader */
void Shader::BindShaderAttribArrays(GLuint meshVAO)
{
  glBindVertexArray(meshVAO);

  /* Create and enable the Vertex's Position pointer/attribute */
  posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
  glEnableVertexAttribArray(posAttrib);

  /* Create and enable the Vertex's UV position pointer/attribute */
  uvAttrib = glGetAttribLocation(shaderProgram, "uv");
  glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
  glEnableVertexAttribArray(uvAttrib);

  /* Create and enable the Vertex's Color position pointer/attribute */
  colorAttrib = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
  glEnableVertexAttribArray(colorAttrib);

  /* Create and enable the Vertex's Normal position pointer/attribute */
  normalAttrib = glGetAttribLocation(shaderProgram, "normal");
  glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(normalAttrib);

  /* set/bind everything back to the OpenGL's default state? */
  //glDisableVertexAttribArray(posAttrib);
  //glDisableVertexAttribArray(uvAttrib);
  //glDisableVertexAttribArray(colorAttrib);
  //glDisableVertexAttribArray(normalAttrib);
  glBindVertexArray(NULL);
}

Shader::Shader(GLuint shaderIndex)
:shaderProgram(shaderIndex)
{
    uniViewProjMatrix = glGetUniformLocation(shaderProgram, "ViewProjMatrix");
    uniModelMatrix = glGetUniformLocation(shaderProgram, "ModelMatrix");
    uniDirLight = glGetUniformLocation(shaderProgram, "lightDirection");
}
