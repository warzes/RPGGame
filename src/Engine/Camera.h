#pragma once

enum CameraMovement
{
	CameraForward,
	CameraBackward,
	CameraLeft,
	CameraRight,
	CameraUp,
	CameraDown,
};

// Default camera values
#ifdef GLM_FORCE_LEFT_HANDED
constexpr float CAMERA_YAW = 90.0f;
#else
constexpr float CAMERA_YAW = -90.0f;// Default yaw to face forward along the negative Z-axis
#endif
constexpr float CAMERA_PITCH = 0.0f;// Default pitch

class Camera final
{
public:
	Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);

	void Move(CameraMovement direction, float speed);
	void Rotate(float pitch, float yaw, float rollDelta);

	glm::mat4 GetViewMatrix();

private:
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraFrontDirection;
	glm::vec3 m_cameraRightDirection;
	glm::vec3 m_cameraUpDirection;
	float     m_yaw = CAMERA_YAW;
	float     m_pitch = CAMERA_PITCH;
};