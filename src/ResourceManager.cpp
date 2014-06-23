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
 
#include "ResourceManager.h"

//"initialize" the instance -- appears redundant at first, but necessary for the singleton to exist/function.
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
	displayList = std::vector<GameObject*>();
}

ResourceManager::~ResourceManager(void)
{
	//delete/call the de-constructors on everything from each map
    for(const auto& element : shaderPairMap)
        glDeleteProgram(element.second);
	shaderPairMap.clear();

    for(const auto& element : meshMap)// learn more specifically why this works
        element.second->~Mesh();
	meshMap.clear();
}

Mesh* ResourceManager::CreateMesh(wchar_t* filename)
{
	/* Retrieve Mesh name from file name (up until the '.') */
	std::wstring file = filename;
	file = file.substr(0,file.find('.'));
	
	/* Don't create the mesh if it already exists */
	if(GetMesh(file.c_str()) != NULL)
		return GetMesh(file.c_str());//this seems in-optimal
	
	/* ----create the new mesh------ */
	Mesh* newMesh = new Mesh(filename);//remember: assigning buffers to mesh is done in the mesh constructor
	
	/* ----store mesh in map-------- */
	meshMap[file]= newMesh;
	
	/*return current Mesh pointer (in case it is needed) */
	return meshMap[file];
}

Mesh* ResourceManager::GetMesh(const wchar_t* meshName)
{
	std::map<std::wstring, Mesh*>::iterator it = meshMap.find(meshName);
	if(it == meshMap.end())
		return NULL;
	return it->second;
}

GLint ResourceManager::CreateShaderProgram(wchar_t* programName,char* const shader1FileName,char* const shader2FileName)
{
	if(GetShaderProgram(programName) != 0)
		return GetShaderProgram(programName);
	
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
	
	/* Store the "pointer" for later */
	shaderPairMap[programName] = myProgram;
	return myProgram;
}

GLint ResourceManager::GetShaderProgram(const wchar_t* programName)
{
	std::map<std::wstring, GLint>::iterator it = shaderPairMap.find(programName);
	if(it == shaderPairMap.end())
		return NULL;
	return it->second;
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
		//returns a bad number (-1)
	}
	return vertShader;
}

GLint ResourceManager::CreateFragmentShader(char* const shaderFileName)
{
	/* Reminder: fragment shader ~= pixel shader */
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	loadShader(fragShader,shaderFileName);
	glCompileShader( fragShader );

	GLint compiled;
	glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		std::cout<<"fragment shader "<< shaderFileName <<" failed to compile"<<std::endl;
		//returns a bad number (-1)
	}
	return fragShader;
}

//TODO: Change the following methods to have more meaningful names?
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
