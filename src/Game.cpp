/*
 * Copyright 2014-2016, Clayton Andrews.
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
/* uncomment for additional debug information */
//#define DEBUG

/* Non-Game (class) Specific Code */
int main(void)
{
	Game* game = new Game();
	game->~Game();//explicitly cleaning up after successful execution
	exit(EXIT_SUCCESS);
}
/* End of Non-Game (class) Specific Code */

Game::~Game()
{
  /* Clean up Game Items -- Non-GLFW/Audio Engine Items Reside Here */
  manager->~ResourceManager();

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
  /* temporary constants for window creation (some config file reading is needed eventually, possibly revisit Lua for this and scripting?) */
  int numSamples = 4;
	bool useAntiAliasing = true;
	width = 800;
	height = 600;

  /* Set hints for use with window creation -- technically optional. TODO: what is GLFW_OPENGL_COMPAT_PROFILE */
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	if(useAntiAliasing)
		glfwWindowHint(GLFW_SAMPLES, numSamples);

	/* Set the error callback - set as early as possible to catch anything when debugging */
	glfwSetErrorCallback(this->error_callback);

	/* Initialize the GLFW library */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "W/A/S/D to Move, Arrow Keys to Rotate, Shift/Control to Change Height, Esc to Terminate", NULL, NULL);
	if (!window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

  /*This function makes the context of the specified window current on the calling thread.
  Note: THIS MUST OCCUR BEFORE INITIALIZING GLEW */
	glfwMakeContextCurrent(window);

	/* Ensures that all extensions with valid entry points will be exposed on experimental drivers */
	glewExperimental = GL_TRUE;

	/* GLEW initialization */
	GLenum err = glewInit();
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

void Game::initializeGame()
{
  /* temporarily store our copies of our pointers in local variables for instantiation*/
  manager = ResourceManager::Instance();
  Mesh* monkeyMesh = manager->CreateMesh(L"assets\\monkey.obj");
  Shader* demoShader = manager->CreateShader(L"demoProgram", "shaders\\vert_shader.glsl", "shaders\\frag_shader.glsl");
  camera = manager->CreateCamera((float)width, (float)height, glm::vec3(0.0f, 0.0f, -20.0f));

  /* Create demo program of spinning monkey heads */
  for (int i = 0; i < 100; i++)
  {
    manager->CreateGameObject(glm::vec3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10), glm::vec3(0,0,0), monkeyMesh, demoShader);
  }
}

void Game::Update()
{
#ifdef DEBUG
	//for printing to the window title (for debugging/cool window names) -- crashed the program in when I was compiling in Ubuntu
	std::stringstream fpsTitle;
	fpsTitle << "This is not the actual frame-rate, but loop cycle speed: " << deltaTime << " ms/cycle";
	glfwSetWindowTitle(window, fpsTitle.str().c_str());
	fpsTitle.str(std::string());//clears the string
#endif

  //controls - should I do this here, or in a callback? this seems to execute "per frame,"
  //where the callback is a kind of messy (statics!)

  // Get mouse position
  //int xpos, ypos;
  //reset mouse position to the center of screen

	//Quit the application
	if((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
    glfwSetWindowShouldClose(window, GL_TRUE);
  //Calculate Camera orientation based on user input
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->Translate(camera->Right(), 20 * deltaTime);//this SHOULD be negative
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->Translate(-camera->Right(), 20 * deltaTime);//this shouldn't be negative
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->Translate(camera->Forward(), 20 * deltaTime);
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->Translate(-camera->Forward(), 20 * deltaTime);
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)//this shouldn't be negative
    camera->Translate(-camera->Up(), 20 * deltaTime);
  if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)//this SHOULD be negative
    camera->Translate(camera->Up(), 20 * deltaTime);
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    camera->Rotate(camera->Up(), -20 * deltaTime, true);//Rotate around Up/"y"; inverted?
  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camera->Rotate(camera->Up(), 20 * deltaTime, true);
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camera->Rotate(camera->Right(),-20 * deltaTime, true);//Rotate around Right/"x"; inverted?
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera->Rotate(camera->Right(),20 * deltaTime, true);
  //end controls code

  //TODO: do a multi-threaded update going through them all
  for (unsigned int i = 0; i < manager->displayList.size(); i++)//also this is functionally cheating since i know whats in the vector
  {
    manager->displayList[i]->transform->Yaw(1000 * deltaTime);
  }
}

void Game::Draw()
{
	/* Check for resize */
	glfwGetFramebufferSize(window, &width, &height);

	/* Handle window resize */
	glViewport(0, 0, width, height);

  /* Update camera and recalculate aspect ratio - need to do something about this when camera management goes to ResourceManager (and multiple cams) */
  camera->SetViewportSize((float)width, (float)height);

	/* Clear the screen (and set a "default" background color) */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* calculate this for each camera -- needed in the shader */
	glm::mat4 ViewProjMatrix = camera->PerspectiveMatrix() * camera->ViewMatrix();

  /* temporary rendering pipeline assuming a fixed camera amount */
  for (unsigned int i = 0; i < manager->displayList.size(); i++)
  {
    /* Ensure we have everything we need to render this object -- should not need to do if set up properly via manager? */

    /* Get the mesh's vertex array object */
    glBindVertexArray(manager->displayList[i]->mesh->vertexObject);

    /* Use the mesh's currently assigned shader */
    glUseProgram(manager->displayList[i]->shader->shaderProgram);

    /*Assign GLSL Uniforms (should these all be uniform?)*/
    glUniformMatrix4fv(manager->displayList[i]->shader->uniViewProjMatrix, 1, GL_FALSE, glm::value_ptr(ViewProjMatrix));
    glUniformMatrix4fv(manager->displayList[i]->shader->uniModelMatrix, 1, GL_FALSE, glm::value_ptr(manager->displayList[i]->CalculateModelMatrix()));
    glUniform3f(manager->displayList[i]->shader->uniDirLight, 0, 0, 1);//hack a light (for now; use lights and calculate light values later)

    //render everything in sequence based on index buffer (?), likes sharing verts/vert re-use
    glDrawElements(GL_TRIANGLES, manager->displayList[i]->mesh->indexCount, GL_UNSIGNED_INT, 0);
  }

	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}

/* Note: Nothing should occur in here "directly" (only in the helper methods) */
Game::Game()
{
	/* Anything pertaining to OpenGL set-up */
	initializeOpenGL();

  /* Set up our audio device/context/etc. here */

	/* Anything pertaining to the actual game initialization should be done here */
	initializeGame();

	/* Get initial time for use in update speed computation */
	lastTime = (float)glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Calculate the value (sort of?) of deltaTime */
		currentTime = (float)glfwGetTime();
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
