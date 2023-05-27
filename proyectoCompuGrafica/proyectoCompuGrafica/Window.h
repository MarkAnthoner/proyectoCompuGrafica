#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();




	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevexHelicoptero() { return muevexHelicoptero; }
	GLfloat getmuevezCarro() { return muevezCarro; }

	int getTiraDado() { return teclaDado; }


	//Funciones de movimiento de Avatar
	GLint getsentido() { return sentido; }	// 0: adelante, 1: atr�s, 2:izquierda, 3:derecha
	GLfloat getmovAvatar() { return movAvatar; }	//0: no se espera movimiento, 1: se solicita movimiento
	void setmovAvatar_r(float a) { movAvatar_r = a; } //0:no se ejecuta alg�n ciclo de movimiento, 1: ciclo en ejecuci�n
	void setmovAvatar(float a) { movAvatar = a; } //0: no se espera movimiento, 1: se solicita movimiento



	GLfloat getapagalinterna() { return apagalinterna; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;


	GLfloat muevex;
	GLfloat muevey;
	GLfloat muevexHelicoptero;
	GLfloat muevezCarro;
	int teclaDado;

	//Movimiento avatar
	GLfloat movAvatar;		//indica que el avatar se mueve, se mantiene activa mientras se completa el ciclo de "paso" sin importar la direcci�n
	GLfloat movAvatar_r;	//bandera de control para que cada ciclo de animaci�n del desplazamiento se complete, evita que se den "pasos dem�s" en una sola pulsacion
	GLint sentido;		//indica en qu� sentido se desplaza el avatar, 0: adelante, 1: atr�s, 2:izquierda, 3:derecha



	GLfloat apagalinterna = 1;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

