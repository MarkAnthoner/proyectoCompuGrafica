#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

#ifndef ANGULO_C
#define ANGULO_C
extern float anguloPersonaje;
extern glm::vec3 posicionPersonaje;

#endif



class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	//Camara isometrica
	void pan(float dx, float dy);
	void zoom(float delta);

	GLint getcameraMode() { return cameraMode; }
	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	//Camara libre
	int cameraMode;
	int habilitaCambioCamara;
	int reinicioCambioCamara;

	//Camara isometrica
	float rotation;     // �ngulo de vista
	float scale;        // Escala de la c�mara
	glm::vec3 frontIsometrica;
	glm::vec3 upIsometrica;
	glm::vec3 positionIsometrica;

	float zoomFactor;  // Factor de zoom inicial
	float zoomSpeed;   // Velocidad de zoom
	int habilitaZoom;

	//Personaje moviendose
	GLfloat distanciaPersonajeCamara;


	void update();
};

