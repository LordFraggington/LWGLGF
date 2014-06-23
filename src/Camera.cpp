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
 
#include "Camera.h"

Camera::Camera(float width, float height, glm::vec3 initialPosition, glm::vec3 initialRotation, float fov, float nearClip, float farClip)
:GameEntity(initialPosition, initialRotation), camFOV(fov), camFarClip(farClip), camNearClip(nearClip)
{
	SetViewportSize(width, height);
	CalculateViewMatrix();
}

Camera::~Camera(void)
{
}

void Camera::SetViewportSize(float width, float height)
{
	//might need to have access to these values later
	camWidth = width;
	camHeight = height;
	camAspectRatio =  camWidth / camHeight;
	CalculatePerspectiveMatrix();
}

void Camera::Translate(glm::vec3 direction, float distance)
{
	position += direction * distance;
	CalculateViewMatrix();
}

void Camera::Rotate(glm::vec3 axis, float degrees,bool localSpace)
{
	glm::quat rotation = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
	if(localSpace)
		orientation = rotation * orientation;
	else
		orientation = orientation * rotation;

	CalculateLocalVectors();
	CalculateViewMatrix();
}

glm::mat4 Camera::PerspectiveMatrix()
{
	return perspectiveMatrix;
}

glm::mat4 Camera::ViewMatrix()
{
	return viewMatrix;
}

void Camera::CalculatePerspectiveMatrix()
{
	perspectiveMatrix = glm::perspective(
			glm::radians(camFOV),
			camAspectRatio,
			camNearClip,
			camFarClip
	);
}

void Camera::CalculateViewMatrix()
{
	/* something about inverses that I cannot remember offhand said to
	 * negate position - double check this */
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), position);
	viewMatrix = glm::toMat4(orientation) * translationMatrix;
}
