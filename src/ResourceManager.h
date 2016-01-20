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
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <unordered_map>
#include "GameObject.h"//Contains Mesh; need to create a display list for these in here
#include "Camera.h"
#include "Light.h"

/*
TODO:
-should use IDs/tags for created object references
-using the location int of the item in the corresponding storage vector, or more like a GUID/globally increasing value over time?
-Would Create___() methods return the ID then?
-Creating shaders needs to take an array of filenames for complex shader programs
-if so, how do we differenciate types of shader
-finish deconstructor
-Cameras technically are transforms, but aren't allocated in the same vector; when Transform is a member variable, handle the problem by NOT calling Tranform's deconstructor in ~Camera
-finish/flesh out CreateCamera method (was originally rapidly written and may be missing important details)
-Turn the filename extraction code in CreateMesh into a method?
-In the Create[Type]Shader methods, return a "bad" number as a flag in the file did not compile
-Clean up loadShader,loadFileToString and glCppShaderSource to look more like the rest of the codebase (casing, meaningful naming)?
-Do Cameras/Lights need names for a lookup table?
*/

/* Reminders for self:
-fragment shader ~= pixel shader
-learn more, specifically, about why the auto loops in the deconstructor work (could I save time doing something else?)
-shaderMap and availableShaders NEED to stay in case multiple materials utilize the same shader.
*/

class ResourceManager
{
	public:
	  static ResourceManager* Instance(void);
		~ResourceManager(void);

    /* Creators */
    Mesh* CreateMesh(wchar_t* filename);
    GameObject* CreateGameObject(glm::vec3 initialPosition = glm::vec3(0, 0, 0), glm::vec3 initialRotation = glm::vec3(0, 0, 0), Mesh* aMesh = NULL, Shader* aShader = NULL);
    Camera* CreateCamera(float weight, float height, glm::vec3 initialPosition = glm::vec3(0, 0, 0));
    Light* CreateLight(glm::vec3 initialPosition = glm::vec3(0, 0, 0), Light::LightType lightType = Light::LightType::Ambient, glm::vec4 inColor = glm::vec4(1, 1, 1, 1),
                        glm::vec3 intensities = glm::vec3(1, 1, 1), float range = 0, float angle = 0, glm::vec3 initialRotation = glm::vec3(0, 0, 0));
    Shader* CreateShader(wchar_t* shaderName, char * const shader1FileName, char * const shader2FileName);

    /* Getters */
    Mesh* GetMesh(const wchar_t* meshName);
    Shader* GetShader(const wchar_t* shaderName);

    /* Actively updated list of things to render */
    std::vector<GameObject*> displayList;

  private:
    //Prevent copies by making these constructors private (don't implement)
    ResourceManager(void);
    ResourceManager(ResourceManager const&);
    void operator=(ResourceManager const&);

    /* Shader creation helper functions -- Shader loading based on instructions from: http://stackoverflow.com/questions/7344640/getting-garbage-chars-when-reading-glsl-files */
    GLint CreateVertexShader(char* const shaderFileName);
    GLint CreateFragmentShader(char* const shaderFileName);
    GLint CreateShaderProgram(char * const shader1FileName, char * const shader2FileName);
    void loadShader(GLuint shaderobject, char* const shadersourcefilename);
    std::string loadFileToString(char const* const shaderfilename);
    void glCppShaderSource(GLuint shader, std::string const &shader_string);

    /* ResourceManager variables */
    static ResourceManager* instance;

    //look-up dictionaries/maps/tables, string to ID
    std::unordered_map<std::wstring, int> meshMap;
    std::unordered_map<std::wstring, int> shaderMap;
    //std::unordered_map<std::wstring, int> textureMap;
    //...etc.

    std::vector<Transform*> activeTransforms;
    std::vector<Light*> activeLights;
    std::vector<Camera*> activeCameras;
    std::vector<Mesh*> availableMeshes;
    std::vector<Shader*> availableShaders;
    //std::vector<Material*> availableMaterials;
    //...etc.
};
#endif
