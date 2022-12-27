#pragma once
#ifndef UTILS
#define UTILS
#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif // 
#include<string>
#include<fstream>
#include<iostream>

GLuint load_shader(const char* filename, GLenum type);
GLuint CreateProgram(GLuint vertexShader, GLuint fragShader);
//@Brief: º”‘ÿÕº∆¨
//@Params: path_to_img
//@Rrturn: texture (GLuint)
GLuint load_texture(const char* img_path);
#endif