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
}

GameObject::GameObject(Transform* aTransform, Mesh* aMesh, Shader* aShader)
{
  transform = aTransform;
	mesh = aMesh;
  shader = aShader;
	currentScale = 1;
  shader->BindShaderAttribArrays(mesh->vertexObject);
}

void GameObject::SetShader(Shader* aShader)
{
  shader = aShader;
  shader->BindShaderAttribArrays(mesh->vertexObject);
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
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), transform->Position());
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(currentScale, currentScale, currentScale));
	glm::mat4 rotationMatrix = glm::toMat4(transform->Orientation());
	/* returns the model's calculated "world" matrix */
	return translationMatrix * rotationMatrix * scaleMatrix;
}
