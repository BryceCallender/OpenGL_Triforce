#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Text.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triforce of Rotation", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to make a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//This contains vec3 of positions and then texture coordinates so we have a vertex array of 2 elements with 3 and 2 floats each
	float vertexAttributes[] =
	{
		//first triangle (bottom left)
		-1.0f, -0.75f, -0.125f, //0
		 0.0f, -0.75f, -0.125f, //1
		-0.5f,  0.0f , -0.125f, //2

		//second triangle (bottom right)
		1.0f, -0.75f, -0.125f,  //3
		0.5f,  0.0f , -0.125f,  //4

		//third triangle  (top)
		0.0f, 0.75f, -0.125f,    //5

		//Fourth (back of first)      
		-1.0f, -0.75f, 0.125f, //6 (0)
		 0.0f, -0.75f, 0.125f, //7 (1) 
		-0.5f,  0.0f , 0.125f, //8 (2)

		//second triangle
		1.0f, -0.75f, 0.125f,  //9 (3)
		0.5f,  0.0f , 0.125f,  //10 (4)

		//third triangle
		0.0f, 0.75f, 0.125f    //11 (5)
	};

	unsigned int bottomLeftTriforceIndicies[] = 
	{
		0, 1, 2 //bottom left	
	};

	unsigned int bottomRightTriforceIndicies[] =
	{
		3, 1, 4 //bottom right
	};

	unsigned int topMiddleTriforceIndicies[] =
	{
		2, 4, 5 //top middle 
	};

	unsigned int backBottomLeftTriforceIndicies[] =
	{
		6, 7, 8 //bottom left	
	};

	unsigned int backBottomRightTriforceIndicies[] =
	{
		9, 7, 10 //bottom right
	};

	unsigned int backTopMiddleTriforceIndicies[] =
	{
		8 , 10, 11 //top middle 
	};

	unsigned int topTriforceQuadLeftIndices[] =
	{
		2, 5, 8,
		5, 8, 11
	};

	unsigned int topTriforceQuadRightIndices[] =
	{
		4, 5, 10,
		5, 10, 11
	};

	unsigned int topTriforceQuadbottomIndices[] =
	{
		2, 4, 10,
		2, 8, 10
	};

	unsigned int bottomLeftTriforceQuadLeftIndices[] =
	{
		0, 2, 8,
		0, 6, 8
	};

	unsigned int bottomLeftTriforceQuadRightIndices[] =
	{
		1, 2, 8,
		1, 7, 8 
	};

	unsigned int bottomLeftTriforceQuadbottomIndices[] =
	{
		0, 1, 7,
		0, 6, 7
	};

	unsigned int bottomRightTriforceQuadLeftIndices[] =
	{
		1, 4, 10,
		1, 7, 10
	};

	unsigned int bottomRightTriforceQuadRightIndices[] =
	{
		3, 4, 10,
		3, 9, 10
	};

	unsigned int bottomRightTriforceQuadbottomIndices[] =
	{
		1, 3, 9,
		1, 7, 9
	};

	Shader triforceShader("vertex.shader", "fragment.shader");
	Shader characterShader("characterVertex.shader", "characterFragment.shader");

	Text text("Fonts/arial.ttf");

	VertexBuffer vertexBufferObject(vertexAttributes, sizeof(vertexAttributes));

	/////////////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer leftTriforceIndexBuffer(bottomLeftTriforceIndicies, 3);
	IndexBuffer rightTriforceIndexBuffer(bottomRightTriforceIndicies, 3);
	IndexBuffer topTriforceIndexBuffer(topMiddleTriforceIndicies, 3);

	IndexBuffer backTopTriforceIndexBuffer(backTopMiddleTriforceIndicies, 3);
	IndexBuffer backBottomLeftTriforceIndexBuffer(backBottomLeftTriforceIndicies, 3);
	IndexBuffer backBottomRightTriforceIndexBuffer(backBottomRightTriforceIndicies, 3);

	/////////////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer topTriforceQuadLeftBuffer(topTriforceQuadLeftIndices, 6);
	IndexBuffer topTriforceQuadRightBuffer(topTriforceQuadRightIndices, 6);
	IndexBuffer topTriforceQuadBottomBuffer(topTriforceQuadbottomIndices, 6);

	IndexBuffer bottomLeftTriforceQuadLeftBuffer(bottomLeftTriforceQuadLeftIndices, 6);
	IndexBuffer bottomLeftTriforceQuadRightBuffer(bottomLeftTriforceQuadRightIndices, 6);
	IndexBuffer bottomLeftTriforceQuadbottomBuffer(bottomLeftTriforceQuadbottomIndices, 6);

	IndexBuffer bottomRightTriforceQuadLeftBuffer(bottomRightTriforceQuadLeftIndices, 6);
	IndexBuffer bottomRightTriforceQuadRightBuffer(bottomRightTriforceQuadRightIndices, 6);
	IndexBuffer bottomRightTriforceQuadbottomBuffer(bottomRightTriforceQuadbottomIndices, 6);

	/////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<VertexArrayAttribute> vertexArrayAtrributes; 

	VertexArrayAttribute vertexAttribute0 = { 3, GL_FLOAT, GL_FALSE, 3, 0 }; // num of floats, type, normalized, stride, offset

	vertexArrayAtrributes.push_back(vertexAttribute0);

	//VertexArrayObject with all these attributes defined
	VertexArray triforceVertexArrayObject(vertexArrayAtrributes);

	//have to bind the VBO so VAO knows about it to draw the indicies
	leftTriforceIndexBuffer.bind();

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render triforces
		//-----------
		glm::mat4 projectionMatrix = glm::perspective(camera.Zoom, (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
		
		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//Get the text shader ready using an orthographic projection matrix
		characterShader.useShader();
		characterShader.setMat4("projection", projection);
		
		//Render the text to the screen using the shader, text, position on screen, scale, and color as parameters
		text.renderText(characterShader, "Triforce", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));

		//Use triforce vertex array and shaders now. No longer using those of the text object
		triforceVertexArrayObject.bind();
		triforceShader.useShader();
		triforceShader.setMat4("projection", projectionMatrix);
		triforceShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		triforceShader.setMat4("model", model);

		////////////////////////////////////////////////////////////////////////////
		//							DRAWS THE FRONT TRIFORCE					 //
		///////////////////////////////////////////////////////////////////////////

		leftTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		rightTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		topTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////////////////
		//							DRAWS THE BACK TRIFORCE						 //
		///////////////////////////////////////////////////////////////////////////

		backTopTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		backBottomLeftTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		backBottomRightTriforceIndexBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////////////////
		//				DRAWS THE FILLING AREA FOR THE TOP TRIFORCE				 //
		///////////////////////////////////////////////////////////////////////////

		topTriforceQuadLeftBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		topTriforceQuadRightBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		topTriforceQuadBottomBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////////////////
		//				DRAWS THE FILLING AREA FOR THE BOTTOM LEFT TRIFORCE		 //
		///////////////////////////////////////////////////////////////////////////

		bottomLeftTriforceQuadLeftBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		bottomLeftTriforceQuadRightBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		bottomLeftTriforceQuadbottomBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////////////////
		//			DRAWS THE FILLING AREA FOR THE BOTTOM RIGHT TRIFORCE		 //
		///////////////////////////////////////////////////////////////////////////

		bottomRightTriforceQuadLeftBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		bottomRightTriforceQuadRightBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		bottomRightTriforceQuadbottomBuffer.bind();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		triforceShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap buffers and stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	//Unbind everything so that it is no longer refering to this in the GPU 
	triforceVertexArrayObject.unbind();

	leftTriforceIndexBuffer.unbind();
	rightTriforceIndexBuffer.unbind();
	topTriforceIndexBuffer.unbind();

	backTopTriforceIndexBuffer.unbind();
	backBottomLeftTriforceIndexBuffer.unbind();
	backBottomRightTriforceIndexBuffer.unbind();

	topTriforceQuadLeftBuffer.unbind();
	topTriforceQuadRightBuffer.unbind();
	topTriforceQuadBottomBuffer.unbind();

	bottomLeftTriforceQuadLeftBuffer.unbind();
	bottomLeftTriforceQuadRightBuffer.unbind();
	bottomLeftTriforceQuadbottomBuffer.unbind();

	bottomRightTriforceQuadLeftBuffer.unbind();
	bottomRightTriforceQuadRightBuffer.unbind();
	bottomRightTriforceQuadbottomBuffer.unbind();

	vertexBufferObject.unbind();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}