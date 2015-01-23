/*
 * Copyright 2014-2015, Clayton Andrews.
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
#ifndef CAMERA_H
#define CAMERA_H
#include "Transform.h"

/*
  TODO:
    -change from inheriting Transform to containing it as a member variable (?)
*/

class Camera : public Transform
{
	public:
		/* Constructor(s) and Deconstructor */
		Camera(float width = 800.0f, float height = 600.0f, glm::vec3 initialPosition = glm::vec3(0,0,0), glm::vec3 initialRotation = glm::vec3(0,0,0), float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.0f);
		~Camera(void);

		/* Overridden Virtual Functions from Transform */
		void Rotate(glm::vec3 axis, float degrees, bool localSpace = false);
		void Translate(glm::vec3 direction, float distance=1);

    /* Camera Specific Functions */
		void SetViewportSize(float width, float height);
		glm::mat4 PerspectiveMatrix();
		glm::mat4 ViewMatrix();
		void CalculateViewMatrix();/* something about inverses that I cannot remember offhand said to negate position - double check this */
		void CalculatePerspectiveMatrix();

	private:
    /* Values used to generate perspective matrix; these are stored for any time we may need to update the pespective matrix */
    float camAspectRatio;
		float camFOV;
		float camNearClip;
		float camFarClip;

    /* Camera Dimensions - needed for multiple cameras in the world, as well as setting the aspect ratio */
    float camWidth;
		float camHeight;

		/* Matrices for rendering models in a scene */
		glm::mat4 perspectiveMatrix;
		glm::mat4 viewMatrix;
};
#endif
