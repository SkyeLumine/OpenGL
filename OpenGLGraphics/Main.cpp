#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);
void copyVerticesToGPU(float vertices[]);
unsigned int createVertexShader();
unsigned int createFragmentShader();
unsigned int createYellowFragmentShader();
unsigned int createProgramShader(unsigned int vertexShader, unsigned int fragmentShader);
void verifyVertexShaderCompilation(unsigned int vertexShader);
void verifyFragmentShaderCompilation(unsigned int fragmentShader);
void verifyShaderProgramLinkage(unsigned int shaderProgram);
void draw(unsigned int shaderProgram, unsigned int VAO);
unsigned int setupVAOWithNoEBO(float vertices[]);


const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main(){\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"};\n";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

const char* yellowFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
	"	FragColor = vec4(0.8f, 0.8f, 0.2f, 1.0f);\n"
	"}\n";

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 

float rectVertices[] = {
	0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f,
     -0.5f,  0.5f, 0.0f
}; 

float dualTrianglesVertices[] = {
	0.0f, 0.0f, 0.0f,  // Center
	-0.5f, 0.0f, 0.0f, // Left triangle -- Bottom Left
	-0.5f, 0.5f, 0.0f, // Left triangle -- Top
	0.0f, 0.5f, 0.0f,  // Right triangle -- Top
	0.5f, 0.0f, 0.0f   // Right triangle -- Bottom Right
};

float leftTriangleVertices[] = {
	0.0f, 0.0f, 0.0f,  // Center
	-0.5f, 0.0f, 0.0f, // Left triangle -- Bottom Left
	-0.5f, 0.5f, 0.0f  // Left triangle -- Top
};

float rightTriangleVertices[] = {
	0.0f, 0.0f, 0.0f,  // Center
	0.0f, 0.5f, 0.0f,  // Right triangle -- Top
	0.5f, 0.0f, 0.0f   // Right triangle -- Bottom Right
};

unsigned int rectIndices[] = {
	0, 1, 3,
	1, 2, 3
};

unsigned int dualTrianglesIndices[] = {
	0, 1, 2, // Left triangle
	0, 3, 4  // Right triangle
};

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}  

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	// Create shaders
	unsigned int vertexShader = createVertexShader();
	unsigned int fragmentShader = createFragmentShader();
	unsigned int yellowfragmentShader = createYellowFragmentShader();
	unsigned int shaderProgram = createProgramShader(vertexShader, fragmentShader);
	unsigned int yellowShaderProgram = createProgramShader(vertexShader, yellowfragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangleVertices), leftTriangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangleVertices), rightTriangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!glfwWindowShouldClose(window))
	{
	    processInput(window);

	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    glUseProgram(shaderProgram);
	    glBindVertexArray(VAO[0]);
	    glDrawArrays(GL_TRIANGLES, 0, 3);

	    glUseProgram(yellowShaderProgram);
	    glBindVertexArray(VAO[1]);
	    glDrawArrays(GL_TRIANGLES, 0, 3);

	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}

	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void copyVerticesToGPU(float vertices[]){
	// An address for a vertex buffer object (VBO)
	unsigned int VBO;

	// Create a vbo and assign its address to VBO
	glGenBuffers(1, &VBO);

	// Bind the buffer as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the vertices into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

unsigned int createVertexShader(){
	// An address for the vertex shader
	unsigned int vertexShader;

	// Create the vertex shader and save its address
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the shader's GLSL source code 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the shader's source code
	glCompileShader(vertexShader);

	verifyVertexShaderCompilation(vertexShader);

	return vertexShader;
}

unsigned int createFragmentShader(){
	// An address for the fragment shader
	unsigned int fragmentShader;

	// Create a fragment shader and save its address (notice same function as vertex shader)
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Attach fragment shader's GLSL source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the shader
	glCompileShader(fragmentShader);

	verifyFragmentShaderCompilation(fragmentShader);

	return fragmentShader;
}

unsigned int createYellowFragmentShader(){
	// An address for the fragment shader
	unsigned int fragmentShader;

	// Create a fragment shader and save its address (notice same function as vertex shader)
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Attach fragment shader's GLSL source code
	glShaderSource(fragmentShader, 1, &yellowFragmentShaderSource, NULL);

	// Compile the shader
	glCompileShader(fragmentShader);

	verifyFragmentShaderCompilation(fragmentShader);

	return fragmentShader;
}

unsigned int createProgramShader(unsigned int vertexShader, unsigned int fragmentShader){
	// Create a shader program to link the vertex and fragment shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach the shaders to the program and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	verifyShaderProgramLinkage(shaderProgram);

	return shaderProgram;
}

void verifyVertexShaderCompilation(unsigned int vertexShader){
	const int logSize = 512;
	int success;
	char infoLog[logSize];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(success){
		std::cout << "Vertex shader compiled successfully!\n";
	} else {
		glGetShaderInfoLog(vertexShader, logSize, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}
}

void verifyFragmentShaderCompilation(unsigned int fragmentShader){
	const int logSize = 512;
	int success;
	char infoLog[logSize];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(success){
		std::cout << "Fragment shader compiled successfully!\n";
	} else {
		glGetShaderInfoLog(fragmentShader, logSize, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}
}

void verifyShaderProgramLinkage(unsigned int shaderProgram){
	const int logSize = 512;
	int success;
	char infoLog[logSize];
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);

	if(success){
		std::cout << "Shader program linked successfully!\n";
	} else {
		glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
		std::cout << "SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}
}

void draw(unsigned int shaderProgram, unsigned int VAO){
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);	
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

unsigned int setupVAOWithNoEBO(float vertices[]){
	
	unsigned int VAO, VBO;

	// Create a Vertex Array Object that will store our vertex attribute config
	glGenVertexArrays(1, &VAO);

	// Create a Vertex Buffer object that will store our vertex data
	glGenBuffers(1, &VBO);

	// Bind VAO so glVertexAttribPointer assigns VBO to it
	glBindVertexArray(VAO);

	// Bind the VBO to the array buffer type and set its vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the VAO's vertex attribute config and enable it
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VAO;
}