#include"viewer.hpp"
#include<iostream>
#include<vector>
#include<Eigen/Core>
static float vertices[] = {
	-0.5f, -0.5f, 0.5f,
	 0.5f, -0.5f, 0.5f,
	 0.5f,  0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,
	-0.5f, -0.5f,-0.5f,
	 0.5f, -0.5f,-0.5f,
	 0.5f,  0.5f,-0.5f,
	-0.5f,  0.5f,-0.5f
};
int learn_context();
int main() {
	learn_context();
	GLViewer::Window window("hello");
	window.loadVertexData(vertices, sizeof(vertices) / sizeof(float));
	window.setCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	glm::mat4 view = window.getCamera().getViewMatrix();
	Eigen::Map<Eigen::Matrix4f> v(glm::value_ptr(view));
	std::cout <<"View:\n" << v << std::endl;
	glm::mat4 proj = window.getCamera().getProjMatrix();
	Eigen::Map<Eigen::Matrix4f> p(glm::value_ptr(proj));
	std::cout <<"Proj:\n" << p << std::endl;
	while (window.isOpen()) {
		window.Render();
	}
	window.Terminate();
}