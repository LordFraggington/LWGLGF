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
#ifndef LIGHT_H
#define LIGHT_H
#include "Transform.h"

/*
TODO:
-See if the intensities should change based on LightType
-Add a volumetric LightType; however, remember that area lighting in most engines involves baking the lightmap...
*/
class Light
{
	public:
	    enum LightType{Ambient, Point, Directional, Spotlight};//TODO: Add volumetric; area lighting in most engines involves baking the lightmap...

		Light(Transform* aTransform = NULL, LightType lightType = LightType::Ambient, glm::vec4 inColor = glm::vec4(1,1,1,1), glm::vec3 intensities = glm::vec3(1,1,1), float range = 0, float angle = 0);
		~Light(void);

    glm::vec3 Direction(); //returns the forward of the Transform; maybe be superfluous but not sure if the naming convention would be useful otherwise

    Transform* transform;

	private:
		LightType currentType;
		float lightAngle;//the angle of the spotlight (affects the width of the resultant circle)
		float lightRange;//the distance the light will travel
		glm::vec4 lightColor;// color of the light and alpha level
		glm::vec3 intensityLevels;// x = ambient, y = diffuse, z = specular; may need to convert to individual floats
};
#endif
