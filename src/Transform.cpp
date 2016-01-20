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

#include "Transform.h"

Transform::~Transform(void)
{
}

Transform::Transform(glm::vec3 initialPosition, glm::vec3 initialRotation)
:position(initialPosition)
{
	orientation = glm::quat();
	glm::quat rotationX = glm::angleAxis(glm::radians(initialRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat rotationY = glm::angleAxis(glm::radians(initialRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotationZ = glm::angleAxis(glm::radians(initialRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	orientation = rotationZ * rotationY * rotationX * orientation;
	CalculateLocalVectors();
}

glm::vec3 Transform::Right()
{
	return rightVector;
}

glm::vec3 Transform::Up()
{
	return upVector;
}

glm::vec3 Transform::Forward()
{
	return forwardVector;
}

glm::vec3 Transform::Position()
{
	return position;
}

glm::quat Transform::Orientation()
{
	return orientation;
}

void Transform::Translate(glm::vec3 direction, float distance)
{
	position += direction * distance;
}

void Transform::Pitch(float degrees)
{
	Rotate(glm::vec3(1.0f, 0.0f ,0.0f), degrees, true);
}

void Transform::Yaw(float degrees)
{
	Rotate(glm::vec3(0.0f, 1.0f, 0.0f), degrees, true);
}

void Transform::Roll(float degrees)
{
	Rotate(glm::vec3(0.0f, 0.0f, 1.0f), degrees, true);
}

void Transform::Rotate(glm::vec3 axis, float degrees,bool localSpace)
{
	glm::quat rotation = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));

	if(localSpace)
		orientation = rotation * orientation;
	else
		orientation = orientation * rotation;

	CalculateLocalVectors();
}

void Transform::CalculateLocalVectors()
{
	/* Derive the local axis vectors from the orientation quaternion. (need to streamline) */
	glm::vec4 rVec = glm::toMat4(glm::angleAxis(glm::pitch(orientation), glm::vec3(1.0f, 0.0f, 0.0f))) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	rightVector = glm::vec3(rVec.x, rVec.y, rVec.z);

	glm::vec4 uVec = glm::toMat4(glm::angleAxis(glm::yaw(orientation), glm::vec3(0.0f, 1.0f, 0.0f))) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	upVector = glm::vec3(uVec.x, uVec.y, uVec.z);

	glm::vec4 fVec = glm::toMat4(glm::angleAxis(glm::roll(orientation), glm::vec3(0.0f, 0.0f, 1.0f))) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	forwardVector = glm::vec3(fVec.x, fVec.y, fVec.z);
}
