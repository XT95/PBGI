#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 	3.14159265358979323846264338328
#endif

#ifndef M_PI_2
#define M_PI_2 	1.57079632679489661923132169164
#endif

class Camera
{
public:
    Camera();
    Camera(const int &, const int &, const glm::vec3 &_position = glm::vec3(0.f, 1.5f, 5.f),
           const float &_horizontalAngle = M_PI, const float &_verticalAngle = 0.,
           const float &_speed = .25f, const float &_mouseSpeed = .005f,
           const float &_fov = 70.f);

	void verticalMovement(const int &);
	void horizontalMovement(const int &);

	void moveForward();

	void moveBackward();

	void moveLeft();

	void moveRight();

	void computeProjection(const int &, const int &);
	
    void setFov(const float &);
    void setViewMatrix(const glm::mat4 &);
    void setProjectionMatrix(const glm::mat4 &);

    glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getViewProjectionMatrix() const;
    glm::vec3 getRight();
    glm::vec3 getUp();
    float getFov() const;
private:
    glm::mat4 viewMatrix, projectionMatrix;
    glm::mat4 viewProjectionMatrix;
	int windowWidth, windowHeight;
    glm::vec3 position, direction, right, up;
	float horizontalAngle, verticalAngle, radius, speed, mouseSpeed, fov;
    static const float minFov = 40.f;
    static const float maxFov = 120.f;
};

#endif // CAMERA_H
