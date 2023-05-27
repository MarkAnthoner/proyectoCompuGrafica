#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	//Movimiento Avatar
	movAvatar = 0;		//no se espera movimiento
	movAvatar_r = 0;	//bandera de bloqueo apagada
	sentido = 0;	//Hacia adelante originalmente


	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	muevey = 2.0f;

	//Movimiento Avatar
	movAvatar = 0;		//no se espera movimiento
	movAvatar_r = 0;	//bandera de bloqueo apagada
	sentido = 0;	//Hacia adelante originalmente

	muevexHelicoptero = 2.0f;
	muevezCarro = 2.0f;

	teclaDado = 0;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto: Diorama - Subnautica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//Movimiento Avatar
	if (theWindow->cameraMode == 0) {
		if (key == GLFW_KEY_W)		//desplazamiento "hacia adelante"
		{
			if (theWindow->movAvatar_r == 0) {
				theWindow->sentido = 0;
				theWindow->movAvatar = 1.0;
				theWindow->movAvatar_r = 1.0;
			}
		}

		if (key == GLFW_KEY_S)		//desplazamiento "hacia atrás"
		{
			if (theWindow->movAvatar_r == 0) {
				theWindow->sentido = 1;
				theWindow->movAvatar = 1.0;
				theWindow->movAvatar_r = 1.0;
			}
		}

		if (key == GLFW_KEY_A)		//desplazamiento "izquierda"
		{
			if (theWindow->movAvatar_r == 0) {
				theWindow->sentido = 2;
				theWindow->movAvatar = 1.0;
				theWindow->movAvatar_r = 1.0;
			}
		}

		if (key == GLFW_KEY_D)
		{
			if (theWindow->movAvatar_r == 0) {
				theWindow->sentido = 3;
				theWindow->movAvatar = 1.0;
				theWindow->movAvatar_r = 1.0;
			}
		}
	}




	//Movimiento del coche adelante y atras en eje X
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}
	//Movimiento del coche en eje Z
	if (key == GLFW_KEY_T)
	{
		theWindow->muevezCarro += 1.0;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->muevezCarro -= 1.0;
	}




	//Movimiento de helicoptero arriba y abajo en eje Y
	if (key == GLFW_KEY_K)
	{
		theWindow->muevey += 1.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->muevey -= 1.0;
	}
	//Movimiento de helicoptero adelante y atras en eje X
	if (key == GLFW_KEY_J)
	{
		theWindow->muevexHelicoptero += 1.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->muevexHelicoptero -= 1.0;
	}



	//Tiro del dado
	if (key == GLFW_KEY_V)
	{
		theWindow->teclaDado += 1;
	}








	if (key == GLFW_KEY_O)
	{
		theWindow->apagalinterna = 1.0;
	}

	if (key == GLFW_KEY_P)
	{
		theWindow->apagalinterna = -1.0;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
