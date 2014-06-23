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
 
#include "Light.h"

Light::~Light(void)
{
	//???
}

Light::Light(LightType lightType, glm::vec3 initialPosition, glm::vec4 color, glm::vec3 intensities, float range, float angle, glm::vec3 initialRotation)
:GameEntity(initialPosition, initialRotation), currentType(lightType), lightAngle(angle), lightRange(range), lightColor(color), intensityLevels(intensities)
{
}

glm::vec3 Light::Direction() //returns the forward of the object; maybe be superfluous but not sure if the naming convention would be useful otherwise
{
	return forwardVector;
}
