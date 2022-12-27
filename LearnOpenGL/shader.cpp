#include"shader.h"
GLlearn::Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_READ_ERROR" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	int success;
	char infolog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMFILE_FAILD\n" << infolog << std::endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMFILE_FAILD\n" << infolog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILD\n" << infolog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void GLlearn::Shader::use() {
	glUseProgram(ID);
}
void GLlearn::Shader::setBool(const std::string& name, bool value)const {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, (int)value);
}
void GLlearn::Shader::setInt(const std::string& name, int value)const {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value);
}
void GLlearn::Shader::setFloat(const std::string& name, float value)const {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, value);
}
GLuint GLlearn::Shader::getProgram() {
	return ID;
}