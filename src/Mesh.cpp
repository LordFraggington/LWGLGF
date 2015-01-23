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
#include "Mesh.h"

Mesh::Mesh(void)
{
}

Mesh::~Mesh(void)
{
	//clear buffers
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);

	//clear buffer object
	glDeleteVertexArrays(1, &vertexObject);

	//clean out pointers
	delete[] vertexArray;
	delete[] indexSequence;
}

/* Note: Presently will only load .objs, though support might be added later on for other formats (namely OpenGEX) */
Mesh::Mesh(wchar_t* filename)
{
	//note, changed to local variables pending possible removal of said variables from header.
	int vertCount, uvCount, normalCount;
	vertCount = uvCount = indexCount = normalCount = 0;
	//temp variables
	std::vector<glm::vec3> verticies;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indicies;
	std::wstring currentLine;

	//create stream
	std::wifstream aStream;
	//open file to parse

	//Uncomment when using MSVC
	//aStream.open(filename);
	//end of MSVC code

	//Uncomment when working with with GCC/MinGW
	char tempBuffer[std::char_traits<wchar_t>::length(filename)*2];
	wcstombs(tempBuffer, filename, sizeof(tempBuffer));
	aStream.open(tempBuffer, std::ios_base::in);
	//end of GCC/MinGW code

	//see if we actually opened a file
	if(aStream.is_open())
	{
		//snag the next line of code
		while(std::getline(aStream,currentLine))
		{
			//create a temp variable for the "type" flag
			std::wstring flag = L"";
			//get the stream for easier manipulation later on
			std::wstringstream currentLineStream(currentLine);
			//get all the data up until the first white-space (1 or 2 characters ultimately)
			currentLineStream >> flag >> std::ws;
			//Vertices with (x,y,z[,w]) coordinates; w is optional and defaults to 1.0.
			if(flag == L"v")
			{
				glm::vec3 tempVect;
				//yank the values from the stream, noting that they are separated by spaces
				currentLineStream >> tempVect.x >> std::ws >> tempVect.y >> std::ws >> tempVect.z; //ignoring W for now
				//add the new vertex to the array
				verticies.push_back(tempVect);
				//vertCount++;
			}
			// parameter space vertices in ( u [,v] [,w] ) form
			/*if(strcmp(currentLine.c_str(),"v") == 0)
			{
			}*/
			//Texture coordinates in (u ,v [,w]) coordinates, these will vary between 0 and 1, w is optional and default to 0.
			else if(flag == L"vt")
			{
				glm::vec2 tempUvVect;
				currentLineStream >> tempUvVect.x >> std::ws >> tempUvVect.y;
				uvs.push_back(tempUvVect);
				//uvCount++;
			}
			//Normals in (x,y,z) form; normals might not be unit normals.
			else if(flag == L"vn")
			{
				glm::vec3 tempNormVect;
				//yank the values from the stream, noting that they are separated by spaces
				currentLineStream >> tempNormVect.x >> std::ws >> tempNormVect.y >> std::ws >> tempNormVect.z;
				//add the new vertex to the array
				normals.push_back(tempNormVect);
				//normalCount++;
			}
			//Face Definitions (1 2 3), (1/2 3/4 5/6), (1/2/3 4/5/6 7/8/9) and (1//2 3//4 5//6) forms.
			//Also, faces can have 3+ points in them
			//ALSO ALSO - objs have a 1-based numbering system instead of 0; remember to subtract 1 from ALL values
			else if(flag == L"f")
			{
				int i = 0;
				while (!currentLineStream.eof())
				{
					int a,b,c;// a is vertex index, b is texture coordinate/UV, c is normal
					a = b = c = 0;
					currentLineStream >> a;
					if (currentLineStream.get() == '/')
					{
						if (currentLineStream.peek() != '/')
						{
							currentLineStream >> b;
							b--;
						}
						if (currentLineStream.get() == '/')
						{
							currentLineStream >> c;
							c--;
						}
					}
					//need to implement storing the other indices
					indicies.push_back(a-1);// REMEMBER: OBJS HAVE A 1-BASED NUMBERING SCHEME
				}
			}
			//else we continue
		}
		aStream.close();
	}
	else
	{
		//throw error
	}
	/* update counts -- if the header variables are removed, refactor these */
	vertCount = verticies.size();
	uvCount = uvs.size();
	normalCount = normals.size();
	indexCount = indicies.size();

	/* Set up intermediary storage pointers */
	indexSequence = new GLuint[indexCount];
	vertexArray = new Vertex[vertCount];

	for(int it = 0; it < indexCount; it++)//there are "always" more indices than vertices (barring degenerate meshes)
	{
		glm::vec2 currentUV;
		glm::vec3 currentNormal;

		indexSequence[it] = (GLuint)indicies[it];

		if(uvCount!= 0 && it < uvCount) currentUV = uvs[it];
		else currentUV = glm::vec2(0,0);

		if(normalCount!= 0 && it < normalCount) currentNormal = normals[it];
		else currentNormal = glm::vec3(0,0,0);

		if(it < vertCount)
		{
			Vertex newVertex = {currentUV,  glm::vec4(1,1,1,1), verticies[it], currentNormal};
			vertexArray[it] = newVertex;
		}
	}

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays(1, &vertexObject);
	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray(vertexObject);

	/* Generate a single vertex buffer */
	glGenBuffers( 1, &vertexBuffer );
	/* Bind our first VBO as being the active buffer and storing vertex attributes */
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/* Copy the vertex data from array to our buffer */
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	/* Generate a single index buffer */
	glGenBuffers( 1, &indexBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indexSequence, GL_STATIC_DRAW);

	/* Unbind VAO for now */
	glBindVertexArray(NULL);
}
