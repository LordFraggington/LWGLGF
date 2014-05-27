#version 330
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

uniform sampler2D myTextureSampler;
//Everything necessary to render from the engine
uniform vec3 lightDirection;//replace with something containing the list of lights?

//Everything needed/contained in the Vertex struct passed to from the vertex shader
in vec2 UV;
in vec4 Color;
in vec3 Normal;

out vec4 outColor;

void main()
{
	// Output color = color of the texture at the specified UV
	// outColor = texture(myTextureSampler, vUV).rgb;
	
	// demo of multiple effects being applied to an arbitrary color (psuedocode, with errors)
	// float3 finalColor = ambientColor*volor+(lightColor*diffuseTerm)+(lightColor*specularTerm);
	// outColor = vec4(finalColor,1);

	//this creates the effect of a directional light
	float intensity = max(dot(Normal,lightDirection), 0.0);
	outColor = Color * intensity;
}
