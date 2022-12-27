#pragma once
#ifndef SHADER_H
#define SHADER_H
#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#define logger(str) (std::cout<<"["<<str<<"]: ")
namespace GLlearn {
	class Shader {
	private:
		std::string vertexCode;
		std::string fragmentCode;
		//³ÌÐòID
		GLuint ID;
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const std::string& name, bool value)const;
		void setInt(const std::string& name, int value)const;
		void setFloat(const std::string& name, float value)const;
		GLuint getProgram();
	};

}
#endif // !SHADER_H
