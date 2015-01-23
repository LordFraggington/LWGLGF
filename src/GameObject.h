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

#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>//used for glm::value_ptr

/*
TODO:
-eliminate direct shader use by using materials
-remove SetShader method and Shader pointer when material system is finished (currently used to swap/set shaders after construction)
-in the long run, we should only actually be assigning a material in this constructor, not a shader, so get rid of the shader addressing there as well
-Should rendering data be private? (doesn't seem to be a good idea)
-Will the deconstructor here actually DO anything?
*/

class GameObject
{
	public:
		GameObject(Transform* aTransform = NULL, Mesh* aMesh = NULL, Shader* aShader = NULL);
		~GameObject(void);

		/* Universal/Uniform Scale */
		void Scale(GLfloat scaleFactor);
		GLfloat CurrentScale();
		glm::mat4 CalculateModelMatrix();

    void SetShader(Shader* aShader);

    /* Variables */
    /* Rendering data */
    Mesh* mesh;
    Shader* shader;
    //Material* currentMaterial;

    Transform* transform;

  private:
    GLfloat currentScale;
};
#endif
