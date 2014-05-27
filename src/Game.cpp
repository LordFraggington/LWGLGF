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
#include "Game.h"
//#define DEBUG //uncomment for additional debug information
//******** "GLOBAL" STUFF ****************************************
int main(void)
{
	Game* game = new Game();
	game->~Game();//just so we are explicitly cleaning up -- do we need to?
	exit(EXIT_SUCCESS);
}
//******** END "GLOBAL" STUFF ************************************
Game::~Game()
{
	/* Clean up game items */
	test->~GameObject();//these need to go into some sort of display list (do a reverse [x--] for-loop for removal)[do this in ResourceManager]
	camera->~Camera();
	light->~Light();
	manager->~ResourceManager();//shut everything down (eventually everything that is not part of glfw/openal will reside here)
	/* Close OpenAL first so sound isn't playing with no window (in case) */
	alcMakeContextCurrent(NULL);
	alcDestroyContext(audioDeviceContext);
	alcCloseDevice(audioDevice);
	/*Close the Window */
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Game::error_callback(int error, const char* description)
{
	/* Write formatted data (string/char*) to the standard error stream */
	fputs(description, stderr);
}

void Game::initializeOpenGL()
{
	//test Lua here to ensure we don't crash something -- move later?
	int numSamples;
	bool useAntiAliasing;
	lua_State *L = luaL_newstate();//calling just lua_open() EXPLODES violently
	luaL_openlibs(L);//load standard lua libs (kind of optional)
	int loaded = luaL_dofile(L, "assets\\config.lua");// load the script (saving the returned flag to check for errors)
	if(loaded == 1)//error
	{
		//TODO: handle file not found by generating a new script for next time and using default values
		std::cout<<"no file found"<<std::endl;
	}
	//TODO: if the config file is bad, generate a new script for next time and use default values
	lua_getglobal(L, "screenWidth");
	if(lua_isnumber(L, 1))
		width = (int)lua_tonumber(L,1);
	lua_getglobal(L, "screenHeight");
	if(lua_isnumber(L, 2))
		height = (int)lua_tonumber(L,2);
	lua_getglobal(L, "antiAliasing");
	if(lua_isboolean(L, 3))
		useAntiAliasing = lua_toboolean(L,3);
	lua_getglobal(L, "numSamples");
	if(lua_isnumber(L, 4))
		numSamples = (int)lua_tonumber(L,4);
	lua_pop(L, 4);//remove x elements from the stack
	lua_close(L);
	//delete L;

	//set hints for use with window creation -- technically optional
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//GLFW_OPENGL_COMPAT_PROFILE <--what are you?!?
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	if(useAntiAliasing)
		glfwWindowHint(GLFW_SAMPLES, numSamples); //Request ???x anti-aliasing

	/* Set the error callback - set as early as possible to catch anything when debugging */
	glfwSetErrorCallback(this->error_callback);

	/* Initialize the GLFW library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "W/A/S/D to Move, Arrow Keys to Rotate, Esc to Terminate", NULL, NULL);
	if (!window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	 /*This function makes the context of the specified window current on the calling thread.
	   THIS MUST OCCUR BEFORE INITIALIZING GLEW */
	glfwMakeContextCurrent(window);

	/* Ensures that all extensions with valid entry points will be exposed on experimental drivers */
	glewExperimental = GL_TRUE;
	/* GLEW initialization */
	GLenum err = glewInit();//the returned enum is either OK or an error
	if (GLEW_OK != err)
	{
		/* Write formatted data to the standard error stream using the literal format string (%s) from err, and print it */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Enable depth */
	glEnable(GL_DEPTH_TEST);
	/* Accept fragment if it closer to the camera than the former one */
	glDepthFunc(GL_LESS);
	/* Cull triangles with normals not facing towards the camera -- on by default(?) */
	glEnable(GL_CULL_FACE);
}

void Game::initializeOpenAL()
{
	/* Initialize device and context (checking to make sure they work) - creating audio stuff comes later! */
	audioDevice = alcOpenDevice(NULL);
	if(!audioDevice)
	{
		/* Write formatted data to the standard error stream */
		fprintf(stderr, "Error: Failed to open audio device.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	audioDeviceContext = alcCreateContext(audioDevice, NULL);
	if(!audioDeviceContext)
	{
		/* Write formatted data to the standard error stream */
		fprintf(stderr, "Error: Failed to obtain audio device context.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
}

void Game::initializeGame()
{
    manager = ResourceManager::Instance();
    camera = new Camera(width, height, glm::vec3(0,0,-2));//facing Z by default; TODO: move camera creation into resource manager
	manager->CreateMesh(L"assets\\monkey.obj");
	demoShader = manager->CreateShaderProgram(L"demoProgram", "shaders\\vert_shader.glsl", "shaders\\frag_shader.glsl");
	test = new GameObject(manager->GetMesh(L"assets\\monkey"), demoShader);//TODO: trim file path from name
	test->Scale(0.15f);
	light = new Light();
}

void Game::Update()
{
#ifdef DEBUG
	//for printing to the window title (for debugging/cool window names) -- crashed the program in when I was compiling in Ubuntu
	std::stringstream fpsTitle;
	fpsTitle << "This is not the actual frame-rate, but loop cycle speed: " << deltaTime << " ms/cycle";
	glfwSetWindowTitle(window, fpsTitle.str().c_str());
	fpsTitle.str(std::string());//clear string
#endif

	//controls - should I do this here, or in a callback? this seems to execute "per frame,"
	//where the callback is a kind of messy (statics!)

	// Get mouse position
	//int xpos, ypos;
	//reset mouse position to the center of screen

	//calculate angles
	if((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))//quit
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->Translate(camera->Right(), deltaTime);//this SHOULD be negative
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->Translate(-camera->Right(), deltaTime);//this shouldn't be negative
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->Translate(camera->Forward(), deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->Translate(-camera->Forward(), deltaTime);
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)//this shouldn't be negative
		camera->Translate(-camera->Up(), deltaTime);
	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)//this SHOULD be negative
		camera->Translate(camera->Up(), deltaTime);
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->Rotate(camera->Up(), -20 * deltaTime, true);//Rotate around Up/"y"; inverted?
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->Rotate(camera->Up(), 20 * deltaTime, true);
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->Rotate(camera->Right(),-20 * deltaTime, true);//Rotate around Right/"x"; inverted?
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->Rotate(camera->Right(),20 * deltaTime, true);
	//end controls code

	test->Yaw(1000 * deltaTime);

    //TODO: do a multi-threaded update going through them all
	/*for(int i = 0; i < manager->displayList.size(); i++)
    {
    )*/
}

void Game::Draw()
{
	/* Check for resize */
	glfwGetFramebufferSize(window, &width, &height);
	/* Handle window resize */
	glViewport(0, 0, width, height);
	/* Update camera and recalculate aspect ratio - need to do something about this when camera management goes to ResourceManager (and multiple cams) */
	camera->SetViewportSize(width, height);
	/* Clear the screen (and set a "default" background color) */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* calculate this for each camera -- needed in the shader */
	glm::mat4 ViewProjMatrix = camera->PerspectiveMatrix() * camera->ViewMatrix();
	/*draw things here -- need to create an actual "GameObject display list" loop for drawing purposes (see below) */
	test->Draw(ViewProjMatrix);//TODO--this needs to go away

    //working on changing this to a more consistent rendering pipeline
    /*for(int i = 0; i < manager->displayList.size(); i++)
    {
        if(dislayList[i]->mesh != null && dislayList[i]->currentMaterial !=null)
        {
            //get a given shader and bind it for use
            glUseProgram(dislayList[i]->currentMaterial->currentShader);
            //Get the mesh's vertex array object
            glBindVertexArray(dislayList[i]->->mesh->vertexObject);
            //assign uniforms (should this be uniform?)
            glUniformMatrix4fv(uniViewProjMatrix, 1, GL_FALSE, glm::value_ptr(ViewProjMatrix));
            glUniformMatrix4fv(uniModelMatrix, 1, GL_FALSE, glm::value_ptr(dislayList[i]->CalculateModelMatrix()));
            glUniform3f(uniDirLight, 0, 0, 1);//hack a light (for now)

            //render everything in sequence based on index buffer (?), likes sharing verts/vert re-use
            glDrawElements(GL_TRIANGLES, dislayList[i]->mesh->indexCount, GL_UNSIGNED_INT, 0);
        }
    }*/

	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}

// Note: Nothing should occur in here "directly" (only in the helper methods)
Game::Game()
{
	/* Anything pertaining to OpenGL set-up */
	initializeOpenGL();
	/* Set up our audio device */
	initializeOpenAL();
	/* Anything pertaining to the actual game initialization should be done here */
	initializeGame();
	/* Get initial time for use in update speed computation */
	lastTime = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Calculate the value (sort of?) of deltaTime */
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		/* Update anything you need to in here*/
		Update();
		/* Render here */
		Draw();
		/* Poll for and process events */
		glfwPollEvents();
	}
}
