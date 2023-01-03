#ifndef CAMERA_H
#define CAMERA_H
#include<glad/glad.h>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
namespace GLViewer {
	class Camera;
	static std::ostream& operator<<(std::ostream& os, Camera& camera);
	/*
	Brief:
	* 该类采用坐标系为右手系，y轴向上，z轴向后.
	* 该类仅度量pitch和yaw，不考虑roll ，并且先旋转Y轴（yaw)在旋转X轴(Pitch)。
	* 为方便使用，Yaw值为前向量Front在XZ投影与X轴负半轴(即使用者前方)角度,从上往下看顺时针为正。
	* 前向量Front.y > 0 时, pitch为正。
	* 相机默认前向量为(0,0,-1)，pitch = 0,yaw = 0.
	*/
	class Camera {
	private:
		float Fov;
		glm::vec3 Position;
		glm::vec3 Front;
		float Pitch;
		float Yaw;
		glm::vec3 Up;

		glm::mat4 viewMat;
		glm::mat4 proj;
		float aspect, zNear, zFar;
	public:
		Camera(
			glm::vec3 position  = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
			float     fov = 45.0f)

		{
			Fov = fov;
			Position = position;
			Front = front;
			Front2Euler();
			updateFromEuler();
			setDefaultProj();
		}
		Camera(
			float posX, float posY, float posZ,
			float frontX = 0, float frontY = 0, float frontZ = -1,
			float fov = 45.0f) 
		{
			Fov = fov;
			Position = glm::vec3(posX, posY, posZ);
			Front = glm::normalize(glm::vec3(frontX, frontY, frontZ));
			Front2Euler();
			updateFromEuler();
			setDefaultProj();
		}
		//使用ZYX方式计算,单位：角度
		Camera(glm::vec3 pos, float pitch, float yaw, float fov = 45.0f) 
		{
			//logger("Info") << "Constructor 3" << std::endl;
			Fov = 45.0;
			Position = pos;
			Pitch = pitch;
			Yaw = yaw;

			updateFromEuler();
			setDefaultProj();
		}
		void Reset() {
			Position = glm::vec3(0.0f, 0.0f, 0.0f);
			Front = glm::vec3(0.0f, 0.0f, -1.0f);
			Fov = 45.0f;
			Front2Euler();
			updateFromEuler();
			setDefaultProj();
		}
		void setPosition(glm::vec3 position) {
			Position = position;
			updateFromEuler();
		}
		void setYaw(float yaw) {
			Yaw = yaw;
			updateFromEuler();
		}
		
		void setPitch(float pitch) {
			Pitch = pitch;
			updateFromEuler();
		}
		void setFrontVec(glm::vec3 front) {
			Front = front;
			Front2Euler();
			updateFromEuler();
		}
		void setFov(float fov) {
			Fov = fov;
			proj = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		}
		//Fov: Deg
		void setProj(float fov, float aspect_ratio, float z_Near = 0.1f, float z_Far = 100.0f) {
			Fov = fov;
			proj = glm::perspective(glm::radians(fov), aspect_ratio, z_Near, z_Far);
		}
		float getYaw(void) {
			return Yaw;
		}
		float getPitch(void) {
			return Pitch;
		}
		float getFov() {
			return Fov;
		}
		glm::vec3 getPosition() {
			return Position;
		}
		glm::vec3 getFrontVec() {
			return Front;
		}
		glm::vec3 getUpVec() {
			//glm::vec4 up = viewMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			////std::cout << "Camera::getUpVec:\nYaw" << Yaw << " " << "Pitch " << Pitch << std::endl;
			//return glm::normalize(glm::vec3(up.x, up.y, up.z));
			return Up;
		}
		glm::mat4 getViewMatrix() {
			return viewMat;
		}
		glm::mat4 getProjMatrix() {
			return proj;
		}
		const float* getViewPtr() {
			const float* ptr = glm::value_ptr(viewMat);
			return ptr;
		}
		const float* getProjPtr() {
			const float* ptr = glm::value_ptr(proj);
			return ptr;
		}
		
	private:
		void updateFromEuler() {
			glm::mat4 trans(1.0f);
			//trans = glm::rotate(trans, glm::radians((float)roll), glm::vec3(0.0f,0.0f,1.0f));
			trans = glm::rotate(trans, glm::radians((float)Yaw), glm::vec3(0.0f, -1.0f, 0.0f));
			trans = glm::rotate(trans, glm::radians((float)Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::vec4 front = trans * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
			glm::vec4 up = trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			Front = glm::normalize(glm::vec3(front.x, front.y, front.z));
			Up = glm::normalize(glm::vec3(up.x, up.y, up.z));
			//std::cout << "Camera::updateFromEuler:\nYaw " << Yaw << " " << "Pitch " << Pitch << std::endl;
			//std::cout << "Front:" << Front.x << " " << Front.y << " " << Front.z << std::endl;
			//std::cout << "Up:" << Up.x << " " << Up.y << " " << Up.z << std::endl;
			viewMat = trans;
			viewMat = glm::lookAt(Position, Position + Front, Up);

		}
		void setDefaultProj() {
			//Fov = 45.0f;
			proj = glm::identity<glm::mat4>();
			aspect = 1.0f;
			zNear = 0.1f;
			zFar = 100.0f;
			proj = glm::perspective(glm::radians(Fov), aspect, zNear, zFar);
		}
		void Front2Euler() {
			glm::vec3 f = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
			Pitch = glm::acos(glm::abs(glm::dot(f, Front)));
			if (Front.y < 0) {
				Pitch = -Pitch;
			}
			if (Front.z == 0) {
				if (Front.x > 0) Yaw = 90;
				else Yaw = -90;
			}
			else if (Front.z >= 0) {
				Yaw = 180 - glm::atan(Front.x / Front.z);
			}
			else {
				Yaw = glm::atan(Front.x / Front.z);
			}
		}
	};
	static std::ostream& operator<<(std::ostream& os, Camera& camera) {
		os << "Pitch: " << camera.getPitch() << ",Yaw: " << camera.getYaw() << std::endl;
		glm::vec3 front = camera.getFrontVec();
		glm::vec3 up = camera.getUpVec();
		os << "Front: " << front.x << " " << front.y << " " << front.z << " " << std::endl;
		os << "Up: " << up.x << " " << up.y << " " << up.z << " " << std::endl;
		return os;
	}

}
#endif