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
#ifndef GAME_H
#define GAME_H
#include <stdio.h> //used to access stderr
#define GLEW_STATIC// needed for GLEW to be linked statically
#include <GL/glew.h>//included in Mesh as well; removing the include here will cause problems though...
#include <GLFW/glfw3.h>//window handling
#include "ResourceManager.h"//contains Mesh, GameObject, Camera, and GLM references
#include <al.h>//OpenAL(-Soft)
#include <alc.h>//AL context

class Game
{
	public:
		Game();
		~Game();

	private:
		void initializeOpenGL();
		void initializeOpenAL();
		void initializeGame();
		void Update();
		void Draw();
		
		/* GLFW callback in case something breaks (you COULD also do key callbacks but they get messy) */
		static void error_callback(int error, const char* description);
		
		/* Screen variables */
		GLFWwindow* window;
		int width, height;
		
		/* Time variables */
		double lastTime;
		double currentTime;
		double deltaTime;
		
		/*Game (Engine) objects */
		ResourceManager* manager;
		GameObject* test;
		Camera* camera;
		Light* light;
		GLuint demoShader;
		
		/* Audio playback variables */
		ALCdevice* audioDevice;
		ALCcontext* audioDeviceContext;
};
#endif
