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
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "GameEntity.h"
#include "Mesh.h"
#include "Material.h"//is this the right place for this
#include <glm/gtc/type_ptr.hpp>//used for glm::value_ptr
class GameObject : public GameEntity
{
	public:
		GameObject(Mesh* aMesh = NULL, GLuint aShader = 0, glm::vec3 initialPosition = glm::vec3(0,0,0), glm::vec3 initialRotation = glm::vec3(0,0,0));//TODO: move shaders out of here
		~GameObject(void);
		void Draw(glm::mat4 matrix);
		void SetShaderProgram(GLuint aShader);//because sometimes we want to swap/set shaders after construction -- should this be moved?
		/* Universal/ Uniform Scale */
		void Scale(GLfloat scaleFactor);
		GLfloat CurrentScale();
		glm::mat4 CalculateModelMatrix();

        //temporarily not private members as part of the imminent rendering change
        Mesh* mesh;
        Material* currentMaterial;
	private:
		GLuint shaderProgram;// should a GameObject have access to this? in DirectX you set the shader(s) then just draw the items in the buffer...
		GLfloat currentScale;

};
#endif
