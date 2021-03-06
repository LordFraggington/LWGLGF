/*
 * Copyright 2014 - 2016, Clayton Andrews.
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
#ifndef TRANSFORM_H
#define TRANSFORM_H
#define GLM_FORCE_RADIANS// GLM flag - otherwise degrees are used, which are deprecated (usable, but still).
#include <glm/glm.hpp>// also in Vertex; do I need it again? (or maybe just import the header here?)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/*
  TODO:
  -Change protected methods/variables to private after ensuring Camera doesn't inherit from Transform

  NOTE:
  -initialRotation values in the Transform constructor are currently meant to be degrees
*/

class Transform
{
	public:
		/* Constructor(s) and Deconstructor */
    Transform(glm::vec3 initialPosition = glm::vec3(0,0,0), glm::vec3 initialRotation = glm::vec3(0,0,0));
		~Transform();

		/* Virtual Functions */
		virtual void Rotate(glm::vec3 axis, float degrees, bool localSpace = false);
		virtual void Translate(glm::vec3 direction, float distance = 1);

		/* Transformations */
		void Pitch(float degrees);	// X axis
		void Yaw(float degrees);	// Y axis
		void Roll(float degrees);	// Z Axis

		/* Getters */
		glm::vec3 Right();
		glm::vec3 Up();
		glm::vec3 Forward();
		glm::vec3 Position();
		glm::quat Orientation();

	protected:
    /* Generates _-vectors based on position and orientation */
		void CalculateLocalVectors();

		/* Variables */
		glm::quat orientation;
		glm::vec3 position;
		glm::vec3 rightVector;
		glm::vec3 upVector;
		glm::vec3 forwardVector;
};
#endif
