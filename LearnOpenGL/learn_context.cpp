//#include"utils.hpp"//shader_load
#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif
#include"shader.h"

#include<Windows.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<Eigen/Core>
#include<Eigen/Geometry>
//std
#include<random>
#include<vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<Windows.h>
//#define logger(str) (std::cout<<"["<<str<<"]: ")
GLuint load_texture(const char* img_path);

//Data
static float vertices[] = {//pos,color,tex
-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//左下
 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//右下
 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,//右上
-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,//左上，Front
-0.5f, -0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
 0.5f, -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
 0.5f,  0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
-0.5f,  0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f //Back
};
static unsigned int indices[] = {
	0, 1, 2,
	0, 2, 3
};
static unsigned int indices2[] = {
	0, 1, 2,
	0, 2, 3,//前
	4, 5, 6,
	4, 6, 7,//后
	0, 1, 5,
	0, 5, 4,//下
	3, 2, 6,
	3, 6, 7,//上
	0, 3, 7,
	0, 7, 4,//左
	1, 2, 6,
	1, 6, 5

};

//glm::vec3 cubePositions[] = {
std::vector<glm::vec3> cubePositions ={
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float fov = 45.0;
double pitch = 0, yaw = -90;
//Callback
void rollcallback(GLFWwindow* window, double xroll, double yroll);
void buffersize_callback(GLFWwindow* window, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, int button,int action,int mods);
void InputProcess(GLFWwindow* window, GLuint program);
void view(GLFWwindow* window, GLint program);
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
	int width = 800, height = 600;
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello Shader", NULL, NULL);
	if (!window) {
		logger("Error") << "Fail" << std::endl;
		glfwTerminate();
		return 0;
	}
	//Camera

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
	//交互
	glfwSetWindowSizeCallback(window, buffersize_callback);
	
	glfwSetScrollCallback(window,rollcallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetCursorPos(window, width / 2.0, height / 2.0);
	GLFWmousebuttonfun;
	GLint transformLoc = glGetUniformLocation(program, "transform");

	
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		InputProcess(window, program);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		view(window, program);

		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
		int i = 0;
		for (const auto& iter : cubePositions) {
			glm::mat4 model(1.0f);//模型空间到世界空间
			model = glm::translate(model, iter);
			float angle = 20.0f * i;
			i++;
			model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime()*1.0f, glm::vec3(1.0f, 0.3f, 0.5f));
			GLint modelLoc = glGetUniformLocation(program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &EBO2);
	glDeleteProgram(program);
	glfwTerminate();
	char in;
	std::cin >> in;
	return 0;
}
void buffersize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void InputProcess(GLFWwindow* window,GLuint program) {
	static float angle = 0;
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
	static int last_q = GLFW_RELEASE;
	if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_RELEASE){
		if (last_q == GLFW_PRESS) {
			uint32_t value;
			value = glfwGetInputMode(window, GLFW_CURSOR);
			if (value == GLFW_CURSOR_NORMAL) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				logger("Info") << "Input:" << "GLFW_CURSOR_DISABLED" << std::endl;
			}
			if (value == GLFW_CURSOR_DISABLED) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				logger("Info") << "Input:" << "GLFW_CURSOR_NORMAL" << std::endl;
			}
		}
		
	}
	last_q = glfwGetKey(window, GLFW_KEY_Q);
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		float cameraSpeed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		}
	}
}
void view(GLFWwindow* window,GLint program){
	int width, height;

	static bool logEnable = TRUE;
	static auto positer = cubePositions.begin();
	
	
	glm::mat4 view(1.0f);//世界空间到摄像头空间
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 proj(1.0f);//透视矩阵
	glfwGetWindowSize(window, &width, &height);
	proj = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
	
	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	if (logEnable) {
		Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::ColMajor>> v(glm::value_ptr(view));
		logger("Info") << "view \n" << v << std::endl;
		Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::ColMajor>> p(glm::value_ptr(proj));
		logger("Info") << "proj \n" << p << std::endl;
	}
	logEnable = 0;
}
void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	static double lastX, lastY;
	static int first = 1;
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		first = 1;
		return;
	}
	
	if (first) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwSetCursorPos(window, width / 2.0, height / 2.0);
		//logger("Info") << " x pos " << xpos << " , y pos " << ypos << std::endl;
		lastX = width / 2.0;
		lastY = height / 2.0;
		first = 0;
		//logger("Info") << "first" << std::endl;
		//logger("Info") << "Front: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
		
		return;
	}
	//logger("Info") << " x pos " << xpos << " , y pos " << ypos << std::endl;
	double xoffset = xpos - lastX;
	double yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	//logger("Info") << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
	//logger("Info") << "lastX: " << lastX << " lastX: " << lastY << std::endl;
	double sens = 0.05;
	xoffset *= sens;
	yoffset *= sens;
	pitch -= yoffset;
	yaw += xoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	//logger("Info") << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
	//logger("Info") << "pitch: " << pitch << " yaw: " << yaw << std::endl;
	//logger("Info") << "Front: " << front.x << " " << front.y << " " << front.z << std::endl;
	return;
}
void rollcallback(GLFWwindow* window, double xroll, double yroll) {
	static double last_x, last_y;
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		return;
	}
	fov += yroll;
	//logger("Info") << " x roll " << xroll << " , y roll " << yroll << std::endl;
	//logger("Info") << "Fov:" << fov << std::endl;
}
void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS) {
		//logger("Info") << "Left Button:" << " action " << action << " mods " << mods << std::endl;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			//logger("Info") << "Input:" << "GLFW_CURSOR_DISABLED" << std::endl;
			return;
		}
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//logger("Info") << "Input:" << "GLFW_CURSOR_NORMAL" << std::endl;
			return;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
	}
	//logger("Info") << "Button: "<<button << " action: " << action << " mods: " << mods << std::endl;
}