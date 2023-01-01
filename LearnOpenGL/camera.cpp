#include<glad/glad.h>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
namespace GLViewer {

	class Camera {
	private:
		float Fov;
		glm::vec3 Position;
		glm::vec3 Up;
		glm::vec3 Front;
		glm::mat4 viewMat;
		glm::mat4 proj;
	public:
		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 front    = glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
			float     fov      = 45.0f)
			: Fov		(fov),
			Position	(position),
			Up			(glm::normalize(up)),
			Front		(glm::normalize(front))
		{
			updateViewMatrix();
			setDefaultProj();
		}
		Camera(
			float posX, float posY, float posZ,
			float frontX, float frontY, float frontZ,
			float upX, float upY, float upZ,
			float fov = 45.0f) {
			Fov = fov;
			Position = glm::vec3(posX, posY, posZ);
			Front = glm::normalize(glm::vec3(frontX, frontY, frontZ));
			Up = glm::normalize(glm::vec3(upX, upY, upZ));
			updateViewMatrix();
			setDefaultProj();
		}
		//使用ZYX方式计算,单位：角度
		Camera(glm::vec3 pos,float pitch,float yaw,float roll = 0,float fov=45.0f){
			Fov = 45.0;
			Position = pos;
			glm::mat4 trans(1.0f);
			trans = glm::rotate(trans, glm::radians((float)roll), glm::vec3(0.0f,0.0f,1.0f));
			trans = glm::rotate(trans, glm::radians((float)yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			trans = glm::rotate(trans, glm::radians((float)pitch), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 front = trans * glm::vec4(0.0f, 0.0f, 1.0f,1.0f);
			glm::vec4 up = trans * glm::vec4(0.0f, 1.0f, 0.0f,1.0f);

			Front = glm::normalize(glm::vec3(front.x, front.y, front.z));
			Up = glm::normalize(glm::vec3(up.x, up.y, up.z));
			viewMat = trans;
			updateViewMatrix();
			setDefaultProj();
		}
		void setProj(float fov,float aspect,float zNear,float zFar) {
			this->Fov = fov;
			glm::perspective(fov, aspect, zNear, zFar);
		}
	private:
		void updateViewMatrix() {
			viewMat = glm::lookAt(Position, Position + Front, Up);
			
		}
		void setDefaultProj() {
			proj = glm::perspective(Fov, 1.0f, 0.01f, 100.0f);
		}
	};
}