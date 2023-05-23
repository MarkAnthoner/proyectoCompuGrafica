#pragma once

#include <glew.h>
#include <glm.hpp>

//superclase, no clase padre
class Light
{
	//todas reciben este tipo de luz
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	~Light();

//variables son protected, las recibe de la clase que hereda
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

