#ifndef VIEW_HPP
#define VIEW_HPP
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"camera.hpp"
namespace GLViewer {
	// Register Class Window with GLFWwindow*
	static std::map<GLFWwindow*, Window*> window_register;
	class Window {
	private:
		GLFWwindow* window;
		GLuint program;
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;
		int useEBO;
		unsigned int vertex_data_length;
		std::vector<double> VertexData;
		//Camera
		Camera camera;
		std::string vertexShaderSource = 
			"#version 330 core"
			"layout(location = 0) in vec3 Pos;"
			//"layout(location = 1) in vec3 color;"
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
			"#version 330 core"
			"in vec4 VertexColor;"
			"out vec4 FragmentColor;"
			"uniform vec4 ourColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
			"void main() {"
			"FragmentColor = ourColor;"
			"}";
	public:
		Window(std::string window_name, int width = 800, int height = 600) {
			window = Initglfw(window_name, width, height);
			glfwMakeContextCurrent(window);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cerr << "[Error]: " << "GLViewer::Window: Cou not init glad." << std::endl;
				glfwTerminate();
			}
			initDefaultProgram();
			setDefaultVAO();
			window_register[window] = this;
		}
		void loadVertexData(float* data,unsigned int length) {
			vertex_data_length = length;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
		}
		friend void defaultMouseCallback(GLFWwindow* window, int button, int action, int mods);
		friend void defaultCursorCallback(GLFWwindow* window, double xpos, double ypos);
		friend void defaultRollCallback(GLFWwindow* window, double xroll, double yroll);
	private:   
		GLFWwindow* Initglfw(std::string window_name, int width, int height) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			GLFWwindow* win = glfwCreateWindow(width, height, "Hello Shader", NULL, NULL);
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
			glShaderSource(vertexShader, 1,&vShader , NULL);
			int success;
			char infolog[512];
			glCompileShader(vertexShader);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
				std::cerr << "ERROR: " << "GLViewer::Window::initDefaultProgram::" << "Vertex Shader ERROR." << std::endl;
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
			}
			glUseProgram(program);
		}
		void updateDefaultProgramUniform(const std::string uniform = {}) {
			if (uniform.compare("model")) {
				GLuint modelLoc = glGetUniformLocation(program, "model");
				std::cout << "ERROR at GLViewer::Window::updateDefaultProgramUniform:"
					<< "can not set model matrix yet" << std::endl;
			}
			if (uniform.compare("view")) {
				GLuint viewLoc = glGetUniformLocation(program, "view");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera.getViewPtr());
			}
			if (uniform.compare("proj")) {
				GLuint projLoc = glGetUniformLocation(program, "proj");
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera.getProjPtr());
			}
		}
		void setDefaultVAO(void) {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3, (void*)0);
			glEnableVertexAttribArray(0);
		}
		void setDefaultCallback() {
			glfwSetMouseButtonCallback(window, defaultMouseCallback);
				GLFWmousebuttonfun;
		}
		
		
	};
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
		Camera& camera = window_register[window]->camera;
		camera.setPitch(camera.getPitch() - yoffset);
		camera.setYaw(camera.getYaw() + xoffset);
		if (camera.getPitch() > 89.0f)
			camera.setPitch(89.0f);
		if (camera.getPitch() < -89.0f)
			camera.setPitch(-89.0f);
		glm::vec3 front;
		front.x = glm::cos(glm::radians(camera.getPitch())) * glm::cos(glm::radians(camera.getYaw()));
		front.y = glm::sin(glm::radians(camera.getPitch()));
		front.z = glm::cos(glm::radians(camera.getPitch())) * glm::sin(glm::radians(camera.getYaw()));
		camera.setFrontVec(glm::normalize(front));
		//logger("Info") << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
		//logger("Info") << "pitch: " << pitch << " yaw: " << yaw << std::endl;
		//logger("Info") << "Front: " << front.x << " " << front.y << " " << front.z << std::endl;
		return;
	}
	void defaultRollCallback(GLFWwindow* window, double xroll, double yroll) {
		static double last_x, last_y;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			return;
		}
		Camera& camera = window_register[window]->camera;
		camera.setFov(camera.getFov() + yroll);
		//logger("Info") << " x roll " << xroll << " , y roll " << yroll << std::endl;
		//logger("Info") << "Fov:" << fov << std::endl;
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
		//logger("Info") << "Button: "<<button << " action: " << action << " mods: " << mods << std::endl;
	}
}
#endif