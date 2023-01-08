#ifndef VIEWER_HPP
#define VIEWER_HPP
#pragma once
#ifndef GL_INCLUDE
#define GL_INCLUDE
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#endif
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"camera.hpp"
namespace GLViewer {
	// Register Class Window with GLFWwindow*
	class Window;
	static std::map<GLFWwindow*, Window*> window_register;
	void defaultCursorCallback(GLFWwindow* window, double xpos, double ypos);
	void defaultRollCallback(GLFWwindow* window, double xroll, double yroll);
	void defaultMouseCallback(GLFWwindow* window, int button, int action, int mods);
	void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	class Window {
	private:
		GLFWwindow* window;
		GLuint program;
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;
		int useEBO;
		unsigned int vertex_data_size;
		std::vector<double> VertexData;
		Camera camera;
		std::string vertexShaderSource =
			"#version 330 core\n"
			"layout(location = 0) in vec3 Pos;"
			"layout(location = 1) in vec3 color;"
			"uniform mat4 model = mat4("
			"1.0f, 0.0f, 0.0f, 0.0f,"
			"0.0f, 1.0f, 0.0f, 0.0f,"
			"0.0f, 0.0f, 1.0f, 0.0f,"
			"0.0f, 0.0f, 0.0f, 1.0f"
			");"
			"uniform mat4 view = mat4("
			"1.0f, 0.0f, 0.0f, 0.0f,"
			"0.0f, 1.0f, 0.0f, 0.0f,"
			"0.0f, 0.0f, 1.0f, 0.0f,"
			"0.0f, 0.0f, 0.0f, 1.0f"
			");"
			"uniform mat4 projection = mat4("
			"1.0f, 0.0f, 0.0f, 0.0f,"
			"0.0f, 1.0f, 0.0f, 0.0f,"
			"0.0f, 0.0f, 1.0f, 0.0f,"
			"0.0f, 0.0f, 0.0f, 1.0f"
			");"
			"out vec4 VertexColor;"
			"void main() {"
			"gl_Position = projection * view * model * vec4(Pos.x, Pos.y, Pos.z, 1.0);"
			"VertexColor = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";
		std::string fragShaderSource =
			"#version 330 core\n"
			"in vec4 VertexColor;"
			"out vec4 FragmentColor;"
			"uniform vec4 ourColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
			"void main() {"
			"FragmentColor = ourColor;"
			"}";
	public:
		//Camera
		
		static void Terminate() {
			glfwTerminate();
		}
		Window(std::string window_name, int width = 800, int height = 600) {
			window = Initglfw(window_name, width, height);
			glfwMakeContextCurrent(window);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cerr << "[Error]: " << "GLViewer::Window: Cou not init glad." << std::endl;
				glfwTerminate();
			}
			glViewport(0, 0, width, height);
			camera.Reset();
			camera.setProj(45.0f, (float)width / (float)height);

			initDefaultProgram();
			setDefaultCallback();
			window_register[window] = this;
		}
		~Window() {
			window_register.erase(window);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteBuffers(1, &VAO);
		}
		void Render() {
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			camera.setProj(camera.getFov(), (float)width / (float)height);
			updateDefaultProgramUniform("view");
			updateDefaultProgramUniform("proj");
			glDrawArrays(GL_TRIANGLES, 0, vertex_data_size);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		int isOpen() {
			return !glfwWindowShouldClose(window);
		}
		int closeWindow() {
			glfwSetWindowShouldClose(window, true);
			glfwHideWindow(window);
		}
		Camera getCamera() {
			return camera;
		}
		void loadVertexData(float* data, unsigned int size) {
			vertex_data_size = size;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
			std::cout << "[Info]: " << "Send " << size * sizeof(float) << " Bytes data to GPU." << std::endl;
			setDefaultVAO();
		}
		void setCamera(glm::vec3 position, glm::vec3 front = glm::vec3(0.0f,0.0f,-1.0f), float     fov = 45.0f) {
			camera.setFov(fov);
			camera.setFrontVec(front);
			camera.setPosition(position);
		}
		friend void defaultMouseCallback(GLFWwindow* window, int button, int action, int mods);
		friend void defaultCursorCallback(GLFWwindow* window, double xpos, double ypos);
		friend void defaultRollCallback(GLFWwindow* window, double xroll, double yroll);
		friend void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	private:
		GLFWwindow* Initglfw(std::string window_name, int width, int height) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			const char* winname = window_name.c_str();
			GLFWwindow* win = glfwCreateWindow(width, height, winname, NULL, NULL);
			if (!win) {
				std::cerr << "[Error]:" << "GLViewer::Window: Glfw init error." << std::endl;
				glfwTerminate();
				return NULL;
			}
			return win;
		}
		void initDefaultProgram(void) {
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const char* vShader = vertexShaderSource.c_str();
			glShaderSource(vertexShader, 1, &vShader, NULL);
			int success;
			char infolog[512];
			glCompileShader(vertexShader);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
				std::cerr << "ERROR: " << "GLViewer::Window::initDefaultProgram::" << "Vertex Shader ERROR." << std::endl;
				std::cerr << infolog << std::endl;
			}
			GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fshader = fragShaderSource.c_str();
			glShaderSource(fragShader, 1, &fshader, NULL);
			glCompileShader(fragShader);
			glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragShader, 512, NULL, infolog);
				std::cerr << "ERROR: " << "GLViewer::Window::initDefaultProgram::"
					<< "Fragment Shader ERROR." << std::endl;
				std::cerr << infolog << std::endl;
			}
			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragShader);
			glLinkProgram(program);
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 512, NULL, infolog);
				std::cerr << "ERROR: " << "GLViewer::Window::initDefaultProgram::"
					<< "Program link ERROR." << std::endl;
				std::cerr << infolog << std::endl;
			}
			glUseProgram(program);
		}
		void updateDefaultProgramUniform(const std::string uniform = {}) {
			if (uniform.compare("model") == 0) {
				GLuint modelLoc = glGetUniformLocation(program, "model");
				std::cout << "ERROR at GLViewer::Window::updateDefaultProgramUniform:"
					<< "can not set model matrix yet" << std::endl;
			}
			if (uniform.compare("view") == 0) {
				GLuint viewLoc = glGetUniformLocation(program, "view");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.getViewPtr());
			}
			if (uniform.compare("proj") == 0) {
				GLuint projLoc = glGetUniformLocation(program, "projection");
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera.getProjPtr());
			}
		}
		void setDefaultVAO(void) {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
		void setDefaultCallback() {
			glfwSetFramebufferSizeCallback(window, defaultFramebufferSizeCallback);
			glfwSetMouseButtonCallback(window, defaultMouseCallback);
			glfwSetCursorPosCallback(window, defaultCursorCallback);
			glfwSetScrollCallback(window, defaultRollCallback);
			glfwSetKeyCallback(window, defaultKeyCallback);
		}
	};
	void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		std::cout << "[Info]:" << "Chang viewport to height " << height << " width " << width << std::endl;
		glViewport(0, 0, width, height);
	}
	void defaultCursorCallback(GLFWwindow* window, double xpos, double ypos) {
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
			lastX = width / 2.0;
			lastY = height / 2.0;
			first = 0;
			return;
		}
		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;
		//std::cout << "[Info]: Window::defaultCursorCallback" 
		//	<< "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
		//std::cout<<"[Info]: Window::defaultCursorCallback" 
		//	<< "lastX: " << lastX << " lastX: " << lastY << std::endl;
		float sens = 0.05;
		xoffset *= sens;
		yoffset *= sens;
		Camera& camera = window_register[window]->camera;
		camera.setPitch(camera.getPitch() - yoffset);
		camera.setYaw(camera.getYaw() + xoffset);
		if (camera.getPitch() > 89.0f)
			camera.setPitch(89.0f);
		if (camera.getPitch() < -89.0f)
			camera.setPitch(-89.0f);
		//std::cout<<"[Info]: <<Window::defaultCursorCallback>> " 
		//	<< "pitch: " << camera.getPitch() << " yaw: " << camera.getYaw() << std::endl;
		return;
	}
	void defaultRollCallback(GLFWwindow* window, double xroll, double yroll) {
		static double last_x, last_y;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			return;
		}
		Camera& camera = window_register[window]->camera;
		camera.setFov(camera.getFov() + (float)yroll);
	}
	void defaultMouseCallback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
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
	}
	void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		
		/*std::cout<<"Info: defaultKeyCallback:\n" 
			<< "key: " << key << " scancode:" << scancode 
		     << " action: " << action << " mods:" << mods << std::endl;
		*/
		const float cameraSpeed = 0.05f;
		Camera& camera = window_register[window]->camera;
		if (action == 1 || action == 2) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
			if (key == GLFW_KEY_W) {
				camera.setPosition(camera.getPosition() + cameraSpeed * camera.getFrontVec());
			}
			if (key == GLFW_KEY_S) {
				camera.setPosition(camera.getPosition() - cameraSpeed * camera.getFrontVec());
			}
			if (key == GLFW_KEY_A) {
				camera.setPosition(camera.getPosition() -
					cameraSpeed * glm::normalize(glm::cross(camera.getFrontVec(), camera.getUpVec())));
				//glm::vec3 Right = glm::normalize(glm::cross(camera.getFrontVec(), camera.getUpVec()));
				//std::cout << "Right Vec:" << Right.x << " " << Right.y << " " << Right.z << std::endl;
			}
			if (key == GLFW_KEY_D) {
				camera.setPosition(camera.getPosition() +
					cameraSpeed * glm::normalize(glm::cross(camera.getFrontVec(), camera.getUpVec())));
				//glm::vec3 Right = glm::normalize(glm::cross(camera.getFrontVec(), camera.getUpVec()));
				//std::cout << "Right Vec:" << Right.x << " " << Right.y << " " << Right.z << std::endl;
			}
		}

	}
}
#endif