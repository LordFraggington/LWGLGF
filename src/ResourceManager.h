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
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <map>
#include "GameObject.h"//Contains Mesh; need to create a display list for these in here
#include "Camera.h"//need to put camera creation/management in here later -- game looks for camera HERE presently
#include "Light.h"

class ResourceManager
{
	public:
	    static ResourceManager* Instance(void);
		~ResourceManager(void);
		
		Mesh* CreateMesh(wchar_t* filename);
		Mesh* GetMesh(const wchar_t* meshName);
		GLint CreateShaderProgram(wchar_t* programName,char * const shader1FileName,char * const shader2FileName);
		GLint GetShaderProgram(const wchar_t* programName);

	private:
	    ResourceManager(void);
	    
	    //Prevent copies by making these constructors private (don't implement)
        ResourceManager(ResourceManager const&);
        void operator=(ResourceManager const&);
        
        /* Shader creation helper functions */
		GLint CreateVertexShader(char* const shaderFileName);
		GLint CreateFragmentShader(char* const shaderFileName);
		
		/* Shader loading based on instructions from:
        http://stackoverflow.com/questions/7344640/getting-garbage-chars-when-reading-glsl-files */
		void loadShader(GLuint shaderobject, char* const shadersourcefilename);
		std::string loadFileToString(char const* const shaderfilename);
		void glCppShaderSource(GLuint shader, std::string const &shader_string);
        
        /* ResourceManager variables */
	    static ResourceManager* instance;
		std::map<std::wstring,Mesh*> meshMap;
		std::map<std::wstring,GLint> shaderPairMap;
		//camera map?
		//texture map?
		//...etc.
		//actively updated list of things to render - lights and gameobjects can only exist here
		std::vector<GameObject*> displayList;
};
#endif
