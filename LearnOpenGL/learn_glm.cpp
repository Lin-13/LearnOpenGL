#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<Eigen/Geometry>
#include<Eigen/Core>
#define logger(str) (logger("[") <<str << "]: ")
//#define MAIN
#ifdef MAIN
int main(void) {
#else
int learn_glm(void) {
#endif // MAIN
	glm::vec4 vec(1,1,1,1);
	glm::mat4 trans(1.0);
	trans = glm::translate(trans, glm::vec3(1.0, 1.0, 1.0));
	vec = trans * vec;
	std::cout << &vec << std::endl;

	std::cout << vec.x <<" "<< vec.y <<" "<< vec.z << std::endl;
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 1.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	Eigen::Projective3f t = Eigen::Projective3f::Identity();
	Eigen::AngleAxisf rot(3.14 / 2, Eigen::Vector3f(1.0, 1.0, 1.0));
	t.rotate(rot);
	t.scale(2);
	t.pretranslate(Eigen::Vector3f(1, 1, 1));
	std::cout << t.matrix() << std::endl;
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}
