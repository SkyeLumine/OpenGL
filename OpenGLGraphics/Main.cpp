#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include "Shader.h"

unsigned int setupVertexData();
GLFWwindow* createWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);

float vertices[] = {
	// positions		// colors
    -0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// Bottom left
     0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// Bottom right
     0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// Top
}; 

float textureCoords[] = {
	0.0f, 0.0f,	// Bottom left
	1.0f, 0.0f,	// Bottom right
	0.5f, 1.0f	// Top
};

float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

int main(){
	
	GLFWwindow* window = createWindow();
	
	Shader shader("VertexShader.vs", "FragmentShader.fs");
	
	unsigned int VAO = setupVertexData();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window))
	{
	    processInput(window);

	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    shader.use();
	    glBindVertexArray(VAO);
	    glDrawArrays(GL_TRIANGLES, 0, 3);

	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}

	glfwTerminate();
}

unsigned int setupVertexData(){
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

GLFWwindow* createWindow(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    exit(1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    exit(1);
	}  

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

