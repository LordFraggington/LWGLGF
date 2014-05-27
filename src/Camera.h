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
#ifndef CAMERA_H
#define CAMERA_H
#include "GameEntity.h"
class Camera : public GameEntity
{
	public:
		Camera(void);
		Camera(float width, float height, glm::vec3 initialPosition = glm::vec3(0,0,0), glm::vec3 initialRotation = glm::vec3(0,0,0), float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.0f); //TODO: get an actual aspect ratio thing going on in the constructor
		~Camera(void);
		void Rotate(glm::vec3 axis, float degrees, bool localSpace = false);
		void Translate(glm::vec3 direction, float distance=1);
		void SetViewportSize(int width, int height);
		glm::mat4 PerspectiveMatrix();
		glm::mat4 ViewMatrix();
		void CalculateViewMatrix();
		void CalculatePerspectiveMatrix();

	private:
		/* used to generate perspective matrix -- saved for modifying the matrix afterwards */
		float camAspectRatio;
		float camFOV;
		float camNearClip;
		float camFarClip;
		/* needed for multiple cameras in the world/set aspect ratio (maybe?) */
		float camWidth;
		float camHeight;
		/* Matrices for rendering models in a scene */
		glm::mat4 perspectiveMatrix;
		glm::mat4 viewMatrix;
};
#endif
