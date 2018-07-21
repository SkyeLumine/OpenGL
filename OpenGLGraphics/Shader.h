#pragma once

#include <glad/glad.h>
#include<fstream>
#include<sstream>
#include<unordered_map>

using std::string;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::unordered_map;

class Shader {

private:
	enum ShaderType { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER};
	unordered_map<ShaderType, string> shaderMap = {{VERTEX, "Vertex"}, {FRAGMENT, "Fragment"}};
	GLuint id;

public:
	Shader(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath);
	void use();
	void setBool(const string & name, const bool value) const;
	void setInt(const string & name, const int value) const;
	void setFloat(const string & name, const float value) const;

	GLuint getId() const{
		return id;
	}

private:
	string readFile(const GLchar* filePath);
	GLuint compileShader(const ShaderType shaderType, const GLchar* sourceCode);
	GLuint createShaderProgram(const GLuint vertexShaderId, const GLuint fragmentShaderId);

};

Shader::Shader(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath){
	const string vertexShaderSrc = readFile(vertexShaderFilePath);
	const string fragmentShaderSrc = readFile(fragmentShaderFilePath);

	const GLuint vertexShaderId = compileShader(VERTEX, vertexShaderSrc.c_str());
	const GLuint fragmentShaderId = compileShader(FRAGMENT, fragmentShaderSrc.c_str());

	id = createShaderProgram(vertexShaderId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

void Shader::use(){
	glUseProgram(id);
}

string Shader::readFile(const GLchar* filePath){

	ifstream file;
	string shaderCode;

	file.exceptions(ifstream::failbit | ifstream::badbit);

	try{
		file.open(filePath);
		stringstream fileStringStream;
		fileStringStream << file.rdbuf();
		file.close();
		shaderCode = fileStringStream.str();
		cout << filePath << " read successfully!" << endl;
	} catch (ifstream::failure exception){
		cerr << "ERROR::SHADER::FAILED_TO_READ_FILE - " << filePath << endl;
		exit(1);
	}

	return shaderCode;
}

unsigned int Shader::compileShader(const ShaderType shaderType, const GLchar* sourceCode){
	const GLint infoLogSize = 512;
	GLchar infoLog[infoLogSize];
	GLint compileStatus;

	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &sourceCode, NULL);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if(!compileStatus){
		glGetShaderInfoLog(shaderId, infoLogSize, NULL, infoLog);
		cerr << "ERROR::SHADER::" << shaderMap[shaderType] << "::COMPILATION_FAILED\n" << infoLog << endl;
		exit(1);
	}

	return shaderId;
}

GLuint Shader::createShaderProgram(const GLuint vertexShaderId, const GLuint fragmentShaderId){
	const GLint infoLogSize = 512;
	GLchar infoLog[infoLogSize];
	GLint linkStatus;

	GLuint shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);
	glLinkProgram(shaderProgramId);

	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linkStatus);
	if(!linkStatus){
		glGetProgramInfoLog(shaderProgramId, infoLogSize, NULL, infoLog);
		cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		exit(1);
	}

	return shaderProgramId;
}

void Shader::setBool(const string & name, const bool value) const {
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, (int) value);
}
void Shader::setInt(const string & name, const int value) const {
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const string & name, const float value) const {
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
}