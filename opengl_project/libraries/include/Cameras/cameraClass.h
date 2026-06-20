#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN

};

enum Camera_Speed {
	RUNNING,
	WALKING

};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float WALKING_SPEED = 2.5f;
const float RUNNING_SPEED = 5.5f;
const float ZOOM = 45.0f;



class Camera
{

public:

	float MouseSensitivity;
	float Yaw;
	float Pitch;
	float WalkingSpeed;
	float RunningSpeed;
	float Zoom;

	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 Front;
	
	Camera(glm::vec3 position, glm::vec3 up);

	void ProcessMouseScroll(float yoffset);

	void ProcessKeyboaurdInput(Camera_Movement Direction, Camera_Speed Speed, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	glm::mat4 GetViewMatrix();


private:

	void updateCameraVectors();


};




#endif
