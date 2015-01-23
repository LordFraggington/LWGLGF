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

#include "ResourceManager.h"

/*"initialize" the instance -- appears redundant at first, but necessary for the singleton to exist/function. */
ResourceManager* ResourceManager::instance = NULL;

ResourceManager* ResourceManager::Instance(void)
{
  if(instance == NULL)
  {
    instance = new ResourceManager();
  }
  return instance;
}

ResourceManager::ResourceManager(void)
{
  //technically initializing all of these is superfluous right? (assuming I did for each vector)
  displayList = std::vector<GameObject*>();
}

ResourceManager::~ResourceManager(void)
{
  /*delete/call the de-constructors on everything from each map */
  for (const auto& element : shaderMap)
    availableShaders[element.second]->~Shader();
  shaderMap.clear();

  for (const auto& element : meshMap)
    availableMeshes[element.second]->~Mesh();
  meshMap.clear();

  for (unsigned int index = activeCameras.size() - 1; index > -1; index--)
    activeCameras[index]->~Camera();

  for (unsigned int index = activeTransforms.size() - 1; index >= -1; index--)
    activeTransforms[index]->~Transform();

  //currently not using these, but for when it's time -- current example cheats so we don't have actual lights
  //for (unsigned int index = activeLights.size() - 1; index >= 0; index--)
  //	activeLights[index]->~Light();
}

GameObject* ResourceManager::CreateGameObject(glm::vec3 initialPosition, glm::vec3 initialRotation, Mesh* aMesh, Shader* aShader)
{
  activeTransforms.push_back(new Transform(initialPosition, initialRotation));
  displayList.push_back(new GameObject(activeTransforms[activeTransforms.size() - 1], aMesh, aShader));
  return displayList[displayList.size() - 1];
}

Light* ResourceManager::CreateLight(glm::vec3 initialPosition, Light::LightType lightType, glm::vec4 inColor, glm::vec3 intensities, float range, float angle, glm::vec3 initialRotation)
{
  activeTransforms.push_back(new Transform(initialPosition, initialRotation));
  activeLights.push_back(new Light(activeTransforms[activeTransforms.size() - 1], lightType, inColor, intensities, range, angle));
  return activeLights[activeLights.size() - 1];
}

Camera* ResourceManager::CreateCamera(float weight, float height, glm::vec3 initialPosition)
{
  activeCameras.push_back(new Camera(weight, height, initialPosition));
  return activeCameras[activeCameras.size()-1];
}

Mesh* ResourceManager::CreateMesh(wchar_t* filename)
{
  /* Retrieve Mesh "name" from file name */
  std::wstring file = filename;
  unsigned lastSlashOfPath = file.find_last_of(L"\\") + 1;
  std::size_t fileExtension = file.find('.') - lastSlashOfPath;
  file = file.substr(lastSlashOfPath, fileExtension);

  /* Don't create the mesh if it already exists */
  Mesh* thisNamedMesh = GetMesh(file.c_str());
  if (thisNamedMesh != NULL)
    return thisNamedMesh;

    /* Create the new mesh; the full filepath is still needed to load the file */
    thisNamedMesh = new Mesh(filename);

    /* Store mesh in vector; store index in map */
    int meshLocation = availableMeshes.size();
    meshMap[file] = meshLocation;
    availableMeshes.push_back(thisNamedMesh);

    /* Return current Mesh pointer (in case it is needed) */
    return availableMeshes[meshLocation];
}

Mesh* ResourceManager::GetMesh(const wchar_t* meshName)
{
  std::unordered_map<std::wstring, int>::iterator it = meshMap.find(meshName);
  if(it == meshMap.end())
    return NULL;
  return availableMeshes[it->second];
}

Shader* ResourceManager::CreateShader(wchar_t* shaderName, char* const shader1FileName, char* const shader2FileName)
{
  Shader* myShader = GetShader(shaderName);
  if (myShader != 0)
    return myShader;

  /* else.... */
  GLint programIndex = CreateShaderProgram(shader1FileName, shader2FileName);
  availableShaders.push_back(new Shader(programIndex));
  shaderMap[shaderName] = availableShaders.size() - 1;
  return availableShaders[shaderMap[shaderName]];
}
GLint ResourceManager::CreateShaderProgram(char* const shader1FileName,char* const shader2FileName)
{
  /* A program "object" is an object to which shader objects can be attached. */
  GLuint myProgram = glCreateProgram();
  GLint vertShader = CreateVertexShader(shader1FileName);
  GLint fragShader = CreateFragmentShader(shader2FileName);
  glAttachShader( myProgram, vertShader);
  glAttachShader( myProgram, fragShader);

  /* Link the program and clean up */
  glLinkProgram(myProgram);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  /* Store the "pointer" to the program inside the Shader struct back in CreateShader */
  return myProgram;
}

Shader* ResourceManager::GetShader(const wchar_t* shaderName)
{
  std::unordered_map<std::wstring, int>::iterator it = shaderMap.find(shaderName);
  if(it == shaderMap.end())
    return NULL;
  return availableShaders[it->second];
}

GLint ResourceManager::CreateVertexShader(char* const shaderFileName)
{
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  loadShader(vertShader, shaderFileName);
  glCompileShader( vertShader );

  GLint compiled;
  glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
  if ( !compiled )
  {
    std::cout<<"vertex shader "<< shaderFileName <<" failed to compile"<<std::endl;
    // Should return a bad number (-1) here
  }
  return vertShader;
}

GLint ResourceManager::CreateFragmentShader(char* const shaderFileName)
{
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  loadShader(fragShader,shaderFileName);
  glCompileShader( fragShader );

  GLint compiled;
  glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
  if ( !compiled )
  {
    std::cout<<"fragment shader "<< shaderFileName <<" failed to compile"<<std::endl;
    // Should return a bad number (-1) here
  }
  return fragShader;
}

void ResourceManager::loadShader(GLuint shaderobject, char* const shadersourcefilename)
{
  glCppShaderSource(shaderobject, loadFileToString(shadersourcefilename));
}

std::string ResourceManager::loadFileToString(char const* const shaderfilename)
{
  std::ifstream ifile(shaderfilename);
  std::string filetext;

  while( ifile.good() )
  {
    std::string line;
    std::getline(ifile, line);
    filetext.append(line + "\n");
  }

  return filetext;
}

void ResourceManager::glCppShaderSource(GLuint shader, std::string const &shader_string)
{
  GLchar const *shader_source = shader_string.c_str();
  GLint const shader_length = shader_string.size();
  glShaderSource(shader, 1, &shader_source, &shader_length);
}
