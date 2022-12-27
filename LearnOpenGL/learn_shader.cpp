#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif
#include"shader.h"
#include<iostream>
#include<fstream>
#include"test_data.hpp"
#include<cmath>
#define logger(str) (std::cout<<"["<<str<<"]: ")

//#define MAIN
#ifdef MAIN
int main() {
#else
int learn_shader(){
#endif
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Shader", NULL, NULL);
	if (!window) {
		logger("Error") << "Fail" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger("Error") << "Cou not init glad" << std::endl;
		glfwTerminate();
		return 0;
	}
	//Shader
	//GLuint vertexShader = load_shader("vertexshader.txt", GL_VERTEX_SHADER);
	//GLuint fragShader = load_shader("fragmentshader.txt", GL_FRAGMENT_SHADER);
	//GLuint program = CreateProgram(vertexShader, fragShader);
	//glDeleteShader(vertexShader); 
	//glDeleteShader(fragShader);
	//glUseProgram(program);
	//Or
	GLlearn::Shader shader("vertexshader.txt", "fragmentshader.txt");
	shader.use();
	GLuint program = shader.getProgram();
	//GLuint ourcolorLocation = glGetUniformLocation(program, "ourColor");
	//Buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	logger("Info") << "VBO:" << sizeof(vertices) << " Bytes data" << std::endl;
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	GLuint EBO2;
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		double green = glfwGetTime();
		green = sin(green) / 2 + 0.5;
		glUseProgram(program);
		//glUniform4f(ourcolorLocation, 0, green, 0, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}