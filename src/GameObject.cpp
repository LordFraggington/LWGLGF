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
 
#include "GameObject.h"

GameObject::~GameObject(void)
{
	//don't delete mesh pointer because it might be used in other objects (ex. the meshMap).
	//TODO: clean up everything we can
}

GameObject::GameObject(Mesh* aMesh, GLuint aShader, glm::vec3 initialPosition, glm::vec3 initialRotation)
:GameEntity(initialPosition, initialRotation)
{
	mesh = aMesh;
	currentScale = 1;
	shaderProgram = aShader;
}

void GameObject::SetShaderProgram(GLuint aShader)
{
	shaderProgram = aShader;
}

GLfloat GameObject::CurrentScale()
{
	return currentScale;
}

void GameObject::Scale(GLfloat scaleFactor)
{
	currentScale *= scaleFactor;
}

glm::mat4 GameObject::CalculateModelMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(currentScale, currentScale, currentScale));
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	/* returns the model's calculated "world" matrix */
	return translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::Draw(glm::mat4 matrix)
{
	/* Ensure we have everything we need to render this object */
	if(mesh == NULL || shaderProgram == 0)//TODO: need to also handle no shader present scenario (program is missing shaders)?/ Move shaders out of here
	{
		return;
	}

	/* Get the mesh's vertex array object */
	glBindVertexArray(mesh->vertexObject);

	/* Use the mesh's currently assigned shader */
	glUseProgram(shaderProgram);

	//assign uniforms (should this be uniform?)
	GLint uniViewProjMatrix = glGetUniformLocation(shaderProgram, "ViewProjMatrix");
	glUniformMatrix4fv(uniViewProjMatrix, 1, GL_FALSE, glm::value_ptr(matrix));
	GLint uniModelMatrix = glGetUniformLocation(shaderProgram, "ModelMatrix");
	glUniformMatrix4fv(uniModelMatrix, 1, GL_FALSE, glm::value_ptr(CalculateModelMatrix()));
	GLint uniDirLight = glGetUniformLocation(shaderProgram, "lightDirection");
	glUniform3f(uniDirLight, 0, 0, 1);//hack a light (for now)

	/* "Get" the mesh's vertex buffer */
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	/* Specify the layout of the vertex data for passing to the shader */
	/* Create and enable the Vertex's Position pointer/attribute */
	GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
	glEnableVertexAttribArray(posAttrib);

	/* Create and enable the Vertex's UV position pointer/attribute */
	GLint uvAttrib = glGetAttribLocation(shaderProgram, "uv");
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
	glEnableVertexAttribArray(uvAttrib);

	/* Create and enable the Vertex's Color position pointer/attribute */
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
	glEnableVertexAttribArray(colorAttrib);

	/* Create and enable the Vertex's Normal position pointer/attribute */
	GLint normAttrib = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(normAttrib);

	/* "Get" the mesh's index buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);

	//render everything in sequence based on index buffer (?), likes sharing verts/vert re-use
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);

	/* set/bind everything back to the OpenGL's default state. */
	glDisableVertexAttribArray(posAttrib);
	glDisableVertexAttribArray(uvAttrib);
	glDisableVertexAttribArray(colorAttrib);
	glDisableVertexAttribArray(normAttrib);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}
