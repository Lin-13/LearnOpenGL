//#include"utils.hpp"//shader_load
#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif
#include"shader.h"
#include<glm/glm.hpp>
#include<iostream>
#include<fstream>
#include<cmath>
#include<Windows.h>
#define logger(str) (std::cout<<"["<<str<<"]: ")
GLuint load_texture(const char* img_path);

//Data
static float vertices[] = {
-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
};
static unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};
static unsigned int indices2[] = {
	0, 1, 2,
	0, 2, 3
};
void buffersize_callback(GLFWwindow* window, int width, int height);
void InputProcess(GLFWwindow* window, GLuint program);
#define MAIN
#ifdef MAIN
int main() {
#else
int learn_context() {
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
	GLlearn::Shader shader("vshader_with_texture.txt", "fshader_with_texture.txt");
	shader.use();
	GLuint program = shader.getProgram();
	//GLuint ourcolorLocation = glGetUniformLocation(program, "ourColor");
	//Buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	logger("Info") << "VBO:" << sizeof(vertices)/sizeof(float) << " Float data." << std::endl;
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	GLuint EBO2;
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	//Texture
	GLuint texture1 = load_texture("D:\\Document\\pictures\\test3.jpg");
	GLuint texture2 = load_texture("D:\\Document\\pictures\\test4.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture1"), 0);
	shader.setInt("ourTexture2", 1);
	//½»»¥
	glfwSetWindowSizeCallback(window, buffersize_callback);
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		InputProcess(window, program);
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

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &EBO2);
	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}
void buffersize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void InputProcess(GLFWwindow* window,GLuint program) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	float alpha;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		GLint location = glGetUniformLocation(program, "alpha");
		glGetUniformfv(program, location, &alpha);
		alpha = alpha + 0.001f;
		if (alpha >= 1) {
			alpha = 1;
		}
		if (alpha <= 0) {
			alpha = 0;
		}
		glUniform1f(location, alpha);
		logger("Info") << "alpha = " << alpha << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		GLint location = glGetUniformLocation(program, "alpha");
		glGetUniformfv(program, location, &alpha);
		alpha = alpha - 0.001f;
		if (alpha >= 1) {
			alpha = 1;
		}
		if (alpha <= 0) {
			alpha = 0;
		}
		glUniform1f(location, alpha);
		logger("Info") << "alpha = " << alpha << std::endl;
	}
}