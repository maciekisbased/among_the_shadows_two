#include "Cameras/cameraClass.h"

Camera::Camera(glm::vec3 position, glm::vec3 up) : MouseSensitivity(SENSITIVITY), Yaw(YAW), Pitch(PITCH), Position(position), WalkingSpeed(WALKING_SPEED), RunningSpeed(RUNNING_SPEED), Zoom(ZOOM), WorldUp(up) 
{
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{// - so when you scorll out it zooms
	Zoom -= (float)yoffset;
	// checks so you can only zoom in a certain amount
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::ProcessKeyboaurdInput(Camera_Movement Direction, Camera_Speed Speed ,float deltaTime)
{
	float velocity = 0.0f;

	if (Speed == WALKING) // if else changes velocity based on Camera_Speed
		velocity = WalkingSpeed * deltaTime;
	else if (Speed == RUNNING)
		velocity = RunningSpeed * deltaTime;

	// if moving right then right unit vector added scaled by velocity
	if (Direction == RIGHT)
		Position += Right * velocity;
	if (Direction == LEFT)
		Position -= Right * velocity;
	if (Direction == FORWARD)
		Position += Front * velocity;
	if (Direction == BACKWARD)
		Position -= Front * velocity;
	if (Direction == UP)
		Position += WorldUp * velocity;
	if (Direction == DOWN)
		Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{	// offset scaled with sensitiviy
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	// yaw depends on x
	// pitch depends on y
	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch) 
	{// stops you from being able to look past 90 degrees up or down
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	// changes the basis for matrix calculation with new yaw and pitch values
	updateCameraVectors();
		
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up); // returns view matrix
}


void Camera::updateCameraVectors()
{
	// sets front vector as initial pointing position of camera which is calculated with Yaw and Pitch values
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// sets up right direction
	Right = glm::normalize(glm::cross(Front, WorldUp));
	// sets up Up direction 
	// up is player up while WorldUp stays constant (0.0f, 1.0f, 0.0f)
	Up = glm::normalize(glm::cross(Right, Front));
	// Note that we normalize the vectors so that mouse movement is faster
	// if vector is longer then that means that mouse movements will also be slower

}

