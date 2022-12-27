#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
//#define MAIN
void FramebufferSizeCallback(GLFWwindow*,int,int);
void ProcesInput(GLFWwindow*);
GLuint VertexShader();
GLuint FragmentShader();
GLuint ShaderProgram();
static float vertices[] = {
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 0.5f,  0.5f, 0.0f,
-0.5f,  0.5f, 0.0f
};
static unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};
static unsigned int indices2[] = {
	0, 1, 2,
	0, 2, 3
};
//#include"test_data.hpp"
#ifdef MAIN
int main(void) {
#else
int _main(void) {
#endif
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL",NULL,NULL);
	if (window == NULL) {
		std::cout << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Fail to init glad" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	
	glViewport(0, 0, 800, 600);

	//VAO �����������
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//���������ö���glVertexAttribPointer֮ǰ��VAO
	//�κζ������Ե��ö���������VAO��
	
	//Vertex Buffer Obj
	//���������ö�������֮ǰ��VBO
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//�󶨻������ȷ����������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	std::cout << "[Info]: " << "Send " << sizeof(vertices) << " Bytes data to GPU." << std::endl;

	//Element Buffer Obj
	unsigned EBO1;
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	std::cout << "[Info]: " << "Send EBO1 " << sizeof(indices) << " Bytes Element data to GPU." << std::endl;
	unsigned int EBO2;
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices2, GL_STATIC_DRAW);
	std::cout << "[Info]: " << "Send EBO2 " << sizeof(indices) << " Bytes Element data to GPU." << std::endl;
	//���ĸ�����ָ���Կ���ι������ݣ�
	//GL_STATIC_DSRAW:���ݼ�������ı�
	//GL_DYNAMIC_DRAW:���ݻ�ı�ܶ�
	//GL_STREAM_DRAW:����ÿ�λ�ִ����ı�
	
	
	//���ö������� VertexAttrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Shader
	GLuint shaderProgram = ShaderProgram();
	glUseProgram(shaderProgram);
	std::cout << "[Info]: " << "Shader program used." << std::endl;
	//��ʼ�����
	while (!glfwWindowShouldClose(window)) {
		ProcesInput(window);
		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		float greenColor = sin(time) / 2.0f + 0.5f;
		GLuint ourcolorLocation = glGetUniformLocation(shaderProgram, "ourcolor");
		glUniform4f(ourcolorLocation, 0.0f, greenColor, 0.0f, 1.0f);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);//����VBO˳������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//ʹ���߿�ģʽ����.GL_FILL/GL_POINT/GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//Delete
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	std::cout <<"[Info]:" << "Chang viewport to height " << height << " width " << width << std::endl;
	glViewport(0, 0, width, height);
}
void ProcesInput(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
		std::cout << "[Info]:" << "Espace pressed." << std::endl;
		glfwSetWindowShouldClose(window, true);
	}
}
//������ɫ��
GLuint VertexShader() {

	const char* vertexShaderSources = {
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 Vertexcolor;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    Vertexcolor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"}\0"
	};

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSources, NULL);
	glCompileShader(vertexShader);

	int success;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cout << "[Error]: " << log << std::endl;
	}
	else {
		std::cout << "[Info]: " << "Vertex Shader Compiled Successfully!" << std::endl;
	}
	char code[512];
	glGetShaderSource(vertexShader, 512, NULL, code);
	//std::cout << "[Code]: " << "<<Vertex Shader>>\n" << code << std::endl;

	return vertexShader;
}
//Ƭ����ɫ���������ɫ
GLuint FragmentShader(void) {
	const char* fragmentShaderSource = {
		"#version 330 core\n"
		"in vec4 Vertexcolor;\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourcolor;\n"
		"void main()\n"
		"{\n"
		//"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"FragColor = Vertexcolor;\n"
		"FragColor = ourcolor;\n"
		"}\0"
	};
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	int success;
	char log[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cout << "[Error]: " << log << std::endl;
	}
	else {
		std::cout << "[Info]: " << "fragment Shader Compiled Successfully!" << std::endl;
	}
	char code[512];
	glGetShaderSource(fragmentShader, 512, NULL, code);
	//std::cout << "[Code]:" << "<<Fragment Shader>>\n" << code << std::endl;
	return fragmentShader;
}
//��ɫ������
GLuint ShaderProgram(void) {
	GLuint vertexShader = VertexShader();
	GLuint fragmentShader = FragmentShader();
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		std::cout << "[Error]: <<link>>\n" << log << std::endl;
	}
	else {
		std::cout << "[Info]: " << "Program linked successfully." << std::endl;
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		
	}
	return shaderProgram;
}