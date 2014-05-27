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
#include "GameEntity.h"

GameEntity::~GameEntity(void)
{
}

GameEntity::GameEntity(glm::vec3 initialPosition, glm::vec3 initialRotation)
:position(initialPosition)
{
	orientation = glm::quat();
	glm::quat rotationX = glm::angleAxis(glm::radians(initialRotation.x), glm::vec3(1,0,0));
	glm::quat rotationY = glm::angleAxis(glm::radians(initialRotation.y), glm::vec3(0,1,0));
	glm::quat rotationZ = glm::angleAxis(glm::radians(initialRotation.z), glm::vec3(0,0,1));
	orientation = rotationX * rotationY * rotationZ * orientation;//TODO: get this in the correct order/check order
	CalculateLocalVectors();
}

glm::vec3 GameEntity::Right()
{
	return rightVector;
}

glm::vec3 GameEntity::Up()
{
	return upVector;
}

glm::vec3 GameEntity::Forward()
{
	return forwardVector;
}

void GameEntity::Translate(glm::vec3 direction, float distance)
{
	position += direction * distance;
}

void GameEntity::Pitch(float degrees)
{
	Rotate(glm::vec3(1,0,0), glm::radians(degrees), true);
}

void GameEntity::Yaw(float degrees)
{
	Rotate(glm::vec3(0,1,0), glm::radians(degrees), true);
}

void GameEntity::Roll(float degrees)
{
	Rotate(glm::vec3(0,0,1), glm::radians(degrees), true);
}

void GameEntity::Rotate(glm::vec3 axis, float degrees,bool localSpace)
{
	/* Create a quaternion rotated around "axis" by "degrees", then normalize it and convert to a matrix */
	glm::quat rotation = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
	if(localSpace)
		orientation = rotation * orientation;
	else
		orientation = orientation * rotation;

	CalculateLocalVectors();
}

void GameEntity::CalculateLocalVectors()
{
	float x = orientation.x;
	float y = orientation.y;
	float z = orientation.z;
	float w = orientation.w;

	/* Derive the local axis vectors from the orientation quaternion. */
	rightVector = glm::vec3(
		1.0f - 2.0f * (y * y + z * z),
		2.0f * (x * y - w * z),
		2.0f * (x * z + w * y)
	);

	upVector = glm::vec3(
		2.0f * (x * y + w * z),
		1.0f - 2.0f * (x * x + z * z),
		2.0f * (y * z - w * x)
	);

	forwardVector = glm::vec3(
		2.0f * (x * z - w * y),
		2.0f * (y * z + w * x),
		1.0f - 2.0f * (x * x + y * y)
	);
}