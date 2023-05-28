/*
Semestre 2023-2
Proyecto: Diorama de la vida cotidiana -  Subnautica
Integrantes
* Castillo Montes Pamela			Gpo. 10
* Hern�ndez Jaimes Rogelio Yael		Gpo. 10
* Sanchez Perez Marco Antonio		Gpo. 01
*/

#define STB_IMAGE_IMPLEMENTATION

//Librerias generales OpenGL
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//Encabezados generales OpenGL
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//Encabezados Iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//Encabezados Animaci�n Keyframes
#include "Archivos.h"
#include "Frame.h"


//numeros random
#include<cstdlib>
using namespace std;


// === Constantes �ngulos === 
const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;

// === Variables Animaci�n Simple === 

float toffsetu = 0.0f;
float toffsetv = 0.0f;

//Movimiento de pengling
float offsetAvanzaPengling;
float offsetGiroPengling;
float penglingOffset;
float movPenglingZ = 0.0f;
float movPenglingX = 0.0f;
float rotaPengling = 0.0f;
bool activarAnimacionPengling;

//Movimiento de Tom Nook
float offsetAvanzaTomNook;
float offsetGiroTomNook;
float tomNookOffset;
float movTomNookZ = 0.0f;
float movTomNookX = 0.0f;
float rotaTomNook = 0.0f;
bool activarAnimacionTomNook;

//Movimiento de la Bolsa de Dinero
float rotacionBolsaEje;
float offsetGiroBolsa;
float movBolsaArribaAbajo;
float offsetBolsaArribaAbajo;
bool activarAnimacionBolsa;

//Animacion de salto de pez
float offsetAvanzaPez;
float offsetGiroPez;
float pezOffset;
float movPezZ = 0.0f;
float movPezX = 0.0f;
float movPezY = 0.0f;
float rotaPez = 0.0f;
float movPezArribaAbajo = 0.0f;
float offsetPezArribaAbajo;

//Movimiento de Robin (Avatar)
float rotaBrazoDerRobin = 0.0f;
float rotaBrazoIzqRobin = 0.0f;
float rotaPiernaDerRobin = 0.0f;
float rotaPiernaIzqRobin = 0.0f;
float rotaPiernaXDerRobin = 0.0f;
float rotaPiernaXIzqRobin = 0.0f;
float offsetRotaPiernaRobin = 5.0f;
float offsetRotaXPiernaRobin = 1.0f;
float offsetRotaBrazoRobin = 5.0f;
int contadorRobin = 0;
float piernaMovRobin = 0.0f; //0: Pierna derecha, 1:Puerna izquierda, indica cual pierna se va a mover
float aperturaRobin = 0.0f; //0: Pierna derecha, 1:Puerna izquierda, indica cual pierna se va a mover
bool auxiliarMovimiento = true;	//indica el sentido de movimiento inicial (ej. el brazo derecho avanza y tras una parte cambia y regresa).

//Animacion de Levyathan
float offsetAvanzaLev;
float offsetGiroLev;
float levOffset;
float movLevZ = 0.0f;
float movLevX = 0.0f;
float movLevY = 0.0f;
float rotaLev = 0.0f;

//Animacion circular
float offsetAvanzaCirculo;
float offsetGiroCirculo;
float circuloOffset;
float movCirculoZ = 0.0f;
float movCirculoX = 0.0f;
float radioCirculo = 0.0f;
float anguloCirculo = 0.0f;


// === Variables Animacion Basica ===
float activarBooleano, habilitarBooleano = 0;


// === Variables Animaci�n Keyframes ===
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

// === Variables CICLO DIA-NOCHE
//float start;
//float nowTime;


// === Variables Lectura de Archivos === 
std::vector<Archivo> listaArchivos;
Archivo archivo1 = Archivo();


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

// === Variables de Textura ===
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture texto;
Texture dadoTexture;

Texture mesaTexture;
Texture aguaTexture;
Texture arenaTexture;
Texture casaTexture;

Texture LeviathanTexture;
Texture PezRojoTexture;
Texture PenglingTexture;
Texture CapsulasTexture;
Texture UmbrellaTexture;
Texture NetTexture;

// === Variables de Modelos ===

Model papalote;

//Iluminaci�n
Model faro;
Model antorcha;

//Pingu
Model Pingu;

//Bolsa de dinero
Model bolsaDinero;

//Leviathan
Model Leviathan_M;

//Tom Nook
Model tomNook;

//Pez
Model PezRojo_M;

//Robin: Avatar
Model BodyRobin;
Model LegRightRobin;
Model LegLeftRobin;
Model ArmRightRobin;
Model ArmLeftRobin;

//Pengling
Model Pengling_M;

//Capsulas
Model  Capsulas_M;

//Silla de playa
Model BeachChair_M;

//Sombrilla
Model Umbrella_M;

//Red
Model Net_M;

//Dron
Model dron;
Model heliceDron;

Skybox skybox;	//skybox de día
Skybox skybox_night; //skybox de noche

// === Materiales ===
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//  === Luz direccional === 
DirectionalLight mainLight_1, mainLight_2, mainLight_3, mainLight_4, mainLight_5; //5 posiciones a lo largo del día
DirectionalLight mainLight_night_1, mainLight_night_2, mainLight_night_3, mainLight_night_4, mainLight_night_5;


//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightsInverso[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//PARA INPUT CON KEYFRAMES, otra instancia del teclado
void inputKeyframes(bool* keys);



/*C�lculo del promedio de las normales para sombreado de Phong
en algunas ocasiones nos ayuda para no tener que declarar las normales manualmente dentro del VAO
*/
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int pistaIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat pistaVertices[] = {
		//x,y,z                 textura          normal en -y
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	// piso mesh [2]
	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	//agave mesh[3]
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	// flechas mesh[4]
	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	//piso de la pista: Mesh[5]
	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(pistaVertices, pistaIndices, 32, 6);
	meshList.push_back(obj6);

}


//Funcion que genera el dado de 8 caras
void CrearDado()
{
	unsigned int cubo_indices[] = {
		// arriba frente
		0, 1, 2,
		// arriba izquierda
		3, 4, 5,
		// arriba atras
		6, 7, 8,
		// arriba derecha
		9, 10, 11,



		// abajo frente
		12, 13, 14,
		// abajo izquierda
		15, 16, 17,
		// abajo atras
		18, 19, 20,
		// arriba derecha
		21, 22, 23,


	};

	// average normals
	GLfloat cubo_vertices[] = {
		// arriba frente
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  0.9f,			0.5f,  0.75f,			-0.44f,	0.0f,	-0.37f,	//0-A
		0.75f, -0.75f,  0.0f,		0.255f,	0.5f,			-0.44f,	0.0f,	-0.37f,	//1-E
		0.75f,  0.75f,  0.0f,		0.737f,	0.5f,			-0.44f,	0.0f,	-0.37f,	//2-F

		// arriba izquierda
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  0.9f,			0.5f,  0.75f,			0.0f,	0.44f,	-0.37f,	//3-A
		-0.75f, -0.75f,  0.0f,		0.026f,	0.75f,			0.0f,	0.44f,	-0.37f,	//4-C
		0.75f, -0.75f,  0.0f,		0.255f,	0.5f,			0.0f,	0.44f,	-0.37f,	//5-E

		// arriba atras
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  0.9f,			0.5f,  0.75f,			0.44f,	0.0f,	-0.37f,	//6-A
		-0.75f, 0.75f,  0.0f,		0.97f,	0.75f,			0.44f,	0.0f,	-0.37f,	//7-D
		-0.75f, -0.75f,  0.0f,		0.737f,	1.0f,			0.44f,	0.0f,	-0.37f,	//8-C

		// arriba derecha
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  0.9f,			0.5f,  0.75f,			0.0f,	-0.44f,	-0.37f,	//9-A
		0.75f,  0.75f,  0.0f,		0.737f,	0.5f,			0.0f,	-0.44f,	-0.37f,	//10-F
		-0.75f, 0.75f,  0.0f,		0.97f,	0.75f,			0.0f,	-0.44f,	-0.37f,	//11-D




		// abajo frente
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  -0.9f,			0.5f,  0.25f,			-0.44f,	0.0f,	0.37f,	//12-B
		0.75f,  -0.75f,  0.0f,		0.255f,	0.5f,			-0.44f,	0.0f,	0.37f,	//13-E
		0.75f, 0.75f,  0.0f,		0.737f,	0.5f,			-0.44f,	0.0f,	0.37f,	//14-F

		// abajo izquierda
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  -0.9f,			0.5f,  0.25f,			0.0f,	0.44f,	0.37f,	//15-B
		-0.75f,  -0.75f,  0.0f,		0.026f,	0.25f,			0.0f,	0.44f,	0.37f,	//16-C
		0.75f, -0.75f,  0.0f,		0.255f, 0.5f,			0.0f,	0.44f,	0.37f,	//17-E

		// abajo atras
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  -0.9f,			0.5f,  0.25f,			0.44f,	0.0f,	0.37f,	//18-B
		-0.75f,  0.75f,  0.0f,		0.737f,	0.0f,			0.44f,	0.0f,	0.37f,	//19-D
		-0.75f, -0.75f,  0.0f,		0.97f,	0.25f,			0.44f,	0.0f,	0.37f,	//20-C

		// abajo derecha
		//x		y		z			S		T				NX		NY		NZ
		0.0f, 0.0f,  -0.9f,			0.5f,  0.25f,			0.0f,	-0.44f,	0.37f,	//21-B
		0.75f,  0.75f,  0.0f,		0.737f,	0.5f,			0.0f,	-0.44f,	0.37f,	//22-F
		-0.75f, 0.75f,  0.0f,		0.97f,	0.25f,			0.0f,	-0.44f,	0.37f,	//23-D

	};

	//Mesh[6]
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}


void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	/*unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};*/
	unsigned int piramide_indices[] = {
		// Base
		0, 1, 2,
		2, 3, 0,
		// Caras laterales
		4, 1, 0,
		4, 2, 1,
		4, 3, 2,
		4, 0, 3
	};

	/*GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};*/
	GLfloat piramide_vertices[] = {
		// Base
		// x     y     z      S      T     NX    NY    NZ
		-0.5f, 0.0f, -0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // 0
		-0.5f, 0.0f,  0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 1
		 0.5f, 0.0f,  0.5f, 0.5f, 1.0f,  0.0f, 1.0f, 0.0f, // 2
		 0.5f, 0.0f, -0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // 3
		 // V�rtice de la punta
		  0.0f, 1.0f,  0.0f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f  // 4
	};

	//mesh[]
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramide_vertices, piramide_indices, 40, 18);
	meshList.push_back(piramide);
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i, coordenada = 3;
	//n�mero de v�rtices ocupados
	int verticesBase = (res) * 3;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	//apuntadores para guardar todos los v�rtices e �ndices generados
	GLfloat* vertices = (GLfloat*)calloc(sizeof(GLfloat*), (verticesBase)+6);
	unsigned int* indices = (unsigned int*)calloc(sizeof(unsigned int*), res + 1);
	//caso inicial para crear el cono
	vertices[0] = 0.0f;
	vertices[1] = 0.5f;
	vertices[2] = 0.0f;
	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices[i + coordenada] = x;
				break;
			case 1:
				vertices[i + coordenada] = y;
				break;
			case 2:
				vertices[i + coordenada] = z;
				break;
			}
		}
		coordenada += 3;
	}
	//se agregan las coordenadas del �ltimo v�rtice para completar la circunferencia
	vertices[coordenada + 1] = R * cos(0) * dt;
	vertices[coordenada + 2] = -0.5f;
	vertices[coordenada + 3] = R * sin(0) * dt;;

	coordenada += 3;

	//se agregan los �ndices
	for (i = 0; i < res + 2; i++) {
		indices[i] = i;
	}

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMesh(vertices, indices, coordenada, res + 2);
	meshList.push_back(cono);
}


//Funci�n crea primitiva: mesa
void CrearMesa()
{
	unsigned int mesa_indices[] = {
		//== Pata Delantera Izquierda ==
		//Front
		0, 1, 2,
		1, 2, 3,
		//Back
		4, 5, 6,
		5, 6, 7,
		//Left
		8, 9, 10,
		9, 10, 11,
		//Right
		12, 13, 14,
		13, 14, 15,
		//Bottom
		16, 17, 18,
		17, 18, 19,
		//== Pata Delantera Derecha ==
		//Front
		20, 21, 22,
		21, 22, 23,
		//Back
		24, 25, 26,
		25, 26, 27,
		//Left
		28, 29, 30,
		29, 30, 31,
		//Right
		32, 33, 34,
		33, 34, 35,
		//Bottom
		36, 37, 38,
		37, 38, 39,

		//== Pata Trasera Izquierda ==
		//Front
		40, 41, 42,
		41, 42, 43,
		//Back
		44, 45, 46,
		45, 46, 47,
		//Left
		48, 49, 50,
		49, 50, 51,
		//Right
		52, 53, 54,
		53, 54, 55,
		//Bottom
		56, 57, 58,
		57, 58, 59,
		//== Pata Trasera Derecha ==
		//Front
		60, 61, 62,
		61, 62, 63,
		//Back
		64, 65, 66,
		65, 66, 67,
		//Left
		68, 69, 70,
		69, 70, 71,
		//Right
		72, 73, 74,
		73, 74, 75,
		//Bottom
		76, 77, 78,
		77, 78, 79,

		//== Mesa ==
		//Front
		80, 81, 82,
		81, 82, 83,
		//Back
		84, 85, 86,
		85, 86, 87,
		//Left
		88, 89, 90,
		89, 90, 91,
		//Right
		92, 93, 94,
		93, 94, 95,
		//Bottom
		96, 97, 98,
		97, 98, 99,
		//Up
		100, 101, 102,
		101, 102, 103,

		//== Decoracion ==
		//Front
		104, 105, 106,
		105, 106, 107,
		//Back
		108, 109, 110,
		109, 110, 111,
		//Left
		112, 113, 114,
		113, 114, 115,
		//Right
		116, 117, 118,
		117, 118, 119,
		//Bottom
		120, 121, 122,
		121, 122, 123,
	};


	GLfloat mesa_vertices[] = {
		//== Pata Delantera Izquierda ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0: A
		0.5f,  0.0f,  0.0f,		1.0f,	0.143f,		0.0f,	0.0f,	-1.0f,	//1: B
		0.0f,  1.5f,  0.0f,		0.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//2: C
		0.5f,  1.5f,  0.0f,		0.0f,	0.143,		0.0f,	0.0f,	-1.0f,	//3: D

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  -0.5f,	1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4: F
		0.5f,  0.0f,  -0.5f,	1.0f,	0.143f,		0.0f,	0.0f,	1.0f,	//5: E
		0.0f,  1.5f,  -0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//6: H
		0.5f,  1.5f,  -0.5f,	0.0f,	0.143,		0.0f,	0.0f,	1.0f,	//7: G

		//Left
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  0.0f,		1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//8: A
		0.0f,  0.0f,  -0.5f,	1.0f,	0.143f,		1.0f,	0.0f,	0.0f,	//9: F
		0.0f,  1.5f,  0.0f,		0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//10: C
		0.0f,  1.5f,  -0.5f,	0.0f,	0.143,		1.0f,	0.0f,	0.0f,	//11: H

		//Right
		//x		y		z		S		T			-NX		-NY		-NZ
		0.5f,  0.0f,  0.0f,		1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//12: B
		0.5f,  0.0f,  -0.5f,	1.0f,	0.143f,		-1.0f,	0.0f,	0.0f,	//13: E
		0.5f,  1.5f,  0.0f,		0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//14: D
		0.5f,  1.5f,  -0.5f,	0.0f,	0.143,		-1.0f,	0.0f,	0.0f,	//15: G

		//Bottom
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//16: A
		0.5f,  0.0f,  0.0f,		1.0f,	0.143f,		0.0f,	1.0f,	0.0f,	//17: B
		0.0f,  0.0f,  -0.5f,	0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//18: F
		0.5f,  0.0f,  -0.5f,	0.0f,	0.143,		0.0f,	1.0f,	0.0f,	//19: E

		//== Pata Delantera Derecha ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//20: I
		3.5f,  0.0f,  0.0f,		1.0f,	0.143f,		0.0f,	0.0f,	-1.0f,	//21: J
		3.0f,  1.5f,  0.0f,		0.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//22: K
		3.5f,  1.5f,  0.0f,		0.0f,	0.143,		0.0f,	0.0f,	-1.0f,	//23: L

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  -0.5f,	1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//24: M
		3.5f,  0.0f,  -0.5f,	1.0f,	0.143f,		0.0f,	0.0f,	1.0f,	//25: N
		3.0f,  1.5f,  -0.5f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//26: O
		3.5f,  1.5f,  -0.5f,	0.0f,	0.143,		0.0f,	0.0f,	1.0f,	//27: P

		//Left
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  0.0f,		1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//28: I
		3.0f,  0.0f,  -0.5f,	1.0f,	0.143f,		1.0f,	0.0f,	0.0f,	//29: M
		3.0f,  1.5f,  0.0f,		0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//30: K
		3.0f,  1.5f,  -0.5f,	0.0f,	0.143,		1.0f,	0.0f,	0.0f,	//31: O

		//Right
		//x		y		z		S		T			-NX		-NY		-NZ
		3.5f,  0.0f,  0.0f,		1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//32: J
		3.5f,  0.0f,  -0.5f,	1.0f,	0.143f,		-1.0f,	0.0f,	0.0f,	//33: N
		3.5f,  1.5f,  0.0f,		0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//34: L
		3.5f,  1.5f,  -0.5f,	0.0f,	0.143,		-1.0f,	0.0f,	0.0f,	//35: P

		//Bottom
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//36: I
		3.5f,  0.0f,  0.0f,		1.0f,	0.143f,		0.0f,	1.0f,	0.0f,	//37: J
		3.0f,  0.0f,  -0.5f,	0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//38: M
		3.5f,  0.0f,  -0.5f,	0.0f,	0.143,		0.0f,	1.0f,	0.0f,	//39: N


		//== Pata Trasera Izquierda ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//40: A
		0.5f,  0.0f,  -3.0f,	1.0f,	0.143f,		0.0f,	0.0f,	-1.0f,	//41: B
		0.0f,  1.5f,  -3.0f,	0.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//42: C
		0.5f,  1.5f,  -3.0f,	0.0f,	0.143,		0.0f,	0.0f,	-1.0f,	//43: D

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  -3.5f,	1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//44: F
		0.5f,  0.0f,  -3.5f,	1.0f,	0.143f,		0.0f,	0.0f,	1.0f,	//45: E
		0.0f,  1.5f,  -3.5f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//46: H
		0.5f,  1.5f,  -3.5f,	0.0f,	0.143,		0.0f,	0.0f,	1.0f,	//47: G

		//Left
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//48: A
		0.0f,  0.0f,  -3.5f,	1.0f,	0.143f,		1.0f,	0.0f,	0.0f,	//49: F
		0.0f,  1.5f,  -3.0f,	0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//50: C
		0.0f,  1.5f,  -3.5f,	0.0f,	0.143,		1.0f,	0.0f,	0.0f,	//51: H

		//Right
		//x		y		z		S		T			-NX		-NY		-NZ
		0.5f,  0.0f,  -3.0f,	1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//52: B
		0.5f,  0.0f,  -3.5f,	1.0f,	0.143f,		-1.0f,	0.0f,	0.0f,	//53: E
		0.5f,  1.5f,  -3.0f,	0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//54: D
		0.5f,  1.5f,  -3.5f,	0.0f,	0.143,		-1.0f,	0.0f,	0.0f,	//55: G

		//Bottom
		//x		y		z		S		T			-NX		-NY		-NZ
		0.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//56: A
		0.5f,  0.0f,  -3.0f,	1.0f,	0.143f,		0.0f,	1.0f,	0.0f,	//57: B
		0.0f,  0.0f,  -3.5f,	0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//58: F
		0.5f,  0.0f,  -3.5f,	0.0f,	0.143,		0.0f,	1.0f,	0.0f,	//59: E

		//== Pata Trasera Derecha ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//60: I
		3.5f,  0.0f,  -3.0f,	1.0f,	0.143f,		0.0f,	0.0f,	-1.0f,	//61: J
		3.0f,  1.5f,  -3.0f,	0.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//62: K
		3.5f,  1.5f,  -3.0f,	0.0f,	0.143,		0.0f,	0.0f,	-1.0f,	//63: L

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  -3.5f,	1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//64: M
		3.5f,  0.0f,  -3.5f,	1.0f,	0.143f,		0.0f,	0.0f,	1.0f,	//65: N
		3.0f,  1.5f,  -3.5f,	0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//66: O
		3.5f,  1.5f,  -3.5f,	0.0f,	0.143,		0.0f,	0.0f,	1.0f,	//67: P

		//Left
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//68: I
		3.0f,  0.0f,  -3.5f,	1.0f,	0.143f,		1.0f,	0.0f,	0.0f,	//69: M
		3.0f,  1.5f,  -3.0f,	0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//70: K
		3.0f,  1.5f,  -3.5f,	0.0f,	0.143,		1.0f,	0.0f,	0.0f,	//71: O

		//Right
		//x		y		z		S		T			-NX		-NY		-NZ
		3.5f,  0.0f,  -3.0f,	1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//72: J
		3.5f,  0.0f,  -3.5f,	1.0f,	0.143f,		-1.0f,	0.0f,	0.0f,	//73: N
		3.5f,  1.5f,  -3.0f,	0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//74: L
		3.5f,  1.5f,  -3.5f,	0.0f,	0.143,		-1.0f,	0.0f,	0.0f,	//75: P

		//Bottom
		//x		y		z		S		T			-NX		-NY		-NZ
		3.0f,  0.0f,  -3.0f,	1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//76: I
		3.5f,  0.0f,  -3.0f,	1.0f,	0.143f,		0.0f,	1.0f,	0.0f,	//77: J
		3.0f,  0.0f,  -3.5f,	0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//78: M
		3.5f,  0.0f,  -3.5f,	0.0f,	0.143,		0.0f,	1.0f,	0.0f,	//79: N

		//== Mesa ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		-0.5f,  1.5f,  0.5f,	0.0f,   0.35f,		0.0f,	0.0f,	-1.0f,	//80: Q
		4.0f,	1.5f,  0.5f,	0.0f,	0.35f,		0.0f,	0.0f,	-1.0f,	//81: R
		-0.5f,	2.0f,  0.5f,	0.0f,	0.64f,		0.0f,	0.0f,	-1.0f,	//82: S
		4.0f,	2.0f,  0.5f,	0.0f,	0.64f,		0.0f,	0.0f,	-1.0f,	//83: T

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		-0.5f,  1.5f,  -4.0f,	0.0f,   0.35f,		0.0f,	0.0f,	1.0f,	//84: U
		4.0f,	1.5f,  -4.0f,	0.0f,	0.35f,		0.0f,	0.0f,	1.0f,	//85: V
		-0.5f,	2.0f,  -4.0f,	0.0f,	0.64f,		0.0f,	0.0f,	1.0f,	//86: W
		4.0f,	2.0f,  -4.0f,	0.0f,	0.64f,		0.0f,	0.0f,	1.0f,	//87: X

		//Left	
		//x		y		z		S		T			-NX		-NY		-NZ
		-0.5f,  1.5f,  0.5f,	0.0f,   0.35f,		1.0f,	0.0f,	0.0f,	//88: Q
		-0.5f,  1.5f,  -4.0f,	0.0f,   0.35f,		1.0f,	0.0f,	0.0f,	//89: U
		-0.5f,	2.0f,  0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,	//90: S
		-0.5f,	2.0f,  -4.0f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,	//91: W

		//Right	
		//x		y		z		S		T			-NX		-NY		-NZ
		4.0f,	1.5f,  0.5f,	0.0f,	0.35f,		-1.0f,	0.0f,	0.0f,	//92: R
		4.0f,	1.5f,  -4.0f,	0.0f,	0.35f,		-1.0f,	0.0f,	0.0f,	//93: V
		4.0f,	2.0f,  0.5f,	0.0f,	0.64f,		-1.0f,	0.0f,	0.0f,	//94: T
		4.0f,	2.0f,  -4.0f,	0.0f,	0.64f,		-1.0f,	0.0f,	0.0f,	//95: X

		//Bottom	
		//x		y		z		S		T			-NX		-NY		-NZ
		-0.5f,  1.5f,  0.5f,	0.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//96: Q
		4.0f,	1.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//97: R
		-0.5f,  1.5f,  -4.0f,	0.0f,   1.0f,		0.0f,	1.0f,	0.0f,	//98: U
		4.0f,	1.5f,  -4.0f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//99: V

		//Up	
		//x		y		z		S		T			-NX		-NY		-NZ
		-0.5f,	2.0f,  0.5f,	0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//100: S
		4.0f,	2.0f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//101: T
		-0.5f,	2.0f,  -4.0f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//102: W
		4.0f,	2.0f,  -4.0f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//103: X

		//== Decoracion ==
		//Front
		//x		y		z		S		T			-NX		-NY		-NZ
		0.25f,  1.25f, -0.25f,	0.0f,   0.35f,		0.0f,	0.0f,	-1.0f,	//104: Y
		3.25f,	1.25f, -0.25f,	0.0f,	0.35f,		0.0f,	0.0f,	-1.0f,	//105: Z
		0.25f,	1.5f,  -0.25f,	0.0f,	0.64f,		0.0f,	0.0f,	-1.0f,	//106: A1
		3.25f,	1.5f,  -0.25f,	0.0f,	0.64f,		0.0f,	0.0f,	-1.0f,	//107: B1

		//Back
		//x		y		z		S		T			-NX		-NY		-NZ
		0.25f,  1.25f, -3.25f,	0.0f,   0.35f,		0.0f,	0.0f,	1.0f,	//108: C1
		3.25f,	1.25f, -3.25f,	0.0f,	0.35f,		0.0f,	0.0f,	1.0f,	//109: D1
		0.25f,	1.5f,  -3.25f,	0.0f,	0.64f,		0.0f,	0.0f,	1.0f,	//110: E1
		3.25f,	1.5f,  -3.25f,	0.0f,	0.64f,		0.0f,	0.0f,	1.0f,	//111: F1

		//Left	
		//x		y		z		S		T			-NX		-NY		-NZ
		0.25f,  1.25f, -0.25f,	0.0f,   0.35f,		1.0f,	0.0f,	0.0f,	//112: Y
		0.25f,  1.25f, -3.25f,	0.0f,   0.35f,		1.0f,	0.0f,	0.0f,	//113: C1
		0.25f,	1.5f,  -0.25f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,	//114: A1
		0.25f,	1.5f,  -3.25f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,	//115: E1

		//Right	
		//x		y		z		S		T			-NX		-NY		-NZ
		3.25f,	1.25f, -0.25f,	0.0f,	0.35f,		-1.0f,	0.0f,	0.0f,	//116: Z
		3.25f,	1.25f, -3.25f,	0.0f,	0.35f,		-1.0f,	0.0f,	0.0f,	//117: D1
		3.25f,	1.5f,  -0.25f,	0.0f,	0.64f,		-1.0f,	0.0f,	0.0f,	//118: B1
		3.25f,	1.5f,  -3.25f,	0.0f,	0.64f,		-1.0f,	0.0f,	0.0f,	//119: F1

		//Bottom	
		//x		y		z		S		T			-NX		-NY		-NZ
		0.25f,  1.25f, -0.25f,	0.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//120: Y
		3.25f,	1.25f, -0.25f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//121: Z
		0.25f,  1.25f, -3.25f,	0.0f,   1.0f,		0.0f,	1.0f,	0.0f,	//122: C1
		3.25f,	1.25f, -3.25f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//123: D1

	};

	Mesh* mesa = new Mesh();
	mesa->CreateMesh(mesa_vertices, mesa_indices, 992, 186);
	meshList.push_back(mesa);

}

//Funci�n crea primitiva: agua
void CrearAgua()
{
	unsigned int agua_indices[] = {
		//Front
		0, 1, 2,
		1, 2, 3,
		//Back
		4, 5, 6,
		5, 6, 7,
		//Left
		8, 9, 10,
		9, 10, 11,
		//Right
		12, 13, 14,
		13, 14, 15,
		//Bottom
		16, 17, 18,
		17, 18, 19,
		//Top
		20, 21, 22,
		21, 22, 23,
	};


	GLfloat agua_vertices[] = {
		//Front
		//x			y		z		S		T			-NX		-NY		-NZ
		0.0f,	 0.0f,  0.0f,		0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0: A
		0.125f,	 0.0f,  0.0f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1: B
		0.0f,	 1.0f,  0.0f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2: C
		0.375f,	 1.0f,  0.0f,		0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3: D

		//Back
		//x		y		z			S		T			-NX		-NY		-NZ
		0.0f,	 0.0f,  -1.0f,		1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4: E
		0.125f,  0.0f,  -1.0f,		1.0f,	0.143f,		0.0f,	0.0f,	1.0f,	//5: F
		0.0f,	 1.0f,  -1.0f,		0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//6: G
		0.375f,  1.0f,  -1.0f,		0.0f,	0.143,		0.0f,	0.0f,	1.0f,	//7: H

		//Left
		//x		y		z			S		T			-NX		-NY		-NZ
		0.0f,	 0.0f,  0.0f,		1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//8: A
		0.0f,	 0.0f,  -1.0f,		1.0f,	0.143f,		1.0f,	0.0f,	0.0f,	//9: E
		0.0f,	 1.0f,  0.0f,		0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//10: C
		0.0f,	 1.0f,  -1.0f,		0.0f,	0.143,		1.0f,	0.0f,	0.0f,	//11: G

		//Right
		//x		y		z			S		T			-NX		-NY		-NZ
		0.125f,	 0.0f,  0.0f,		1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//12: B
		0.125f,  0.0f,  -1.0f,		1.0f,	0.143f,		-1.0f,	0.0f,	0.0f,	//13: F
		0.375f,	 1.0f,  0.0f,		0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//14: D
		0.375f,  1.0f,  -1.0f,		0.0f,	0.143,		-1.0f,	0.0f,	0.0f,	//15: H

		//Bottom
		//x		y		z			S		T			-NX		-NY		-NZ
		0.0f,	 0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//16: A
		0.125f,	 0.0f,  0.0f,		1.0f,	0.143f,		0.0f,	1.0f,	0.0f,	//17: B
		0.0f,	 0.0f,  -1.0f,		0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//18: E
		0.125f,  0.0f,  -1.0f,		0.0f,	0.143,		0.0f,	1.0f,	0.0f,	//19: F

		//Top
		//x		y		z			S		T			-NX		-NY		-NZ
		0.0f,	 1.0f,  0.0f,		1.0f,   0.0f,		0.0f,	-1.0f,	0.0f,	//20: C
		0.375f,	 1.0f,  0.0f,		1.0f,	0.143f,		0.0f,	-1.0f,	0.0f,	//21: D
		0.0f,	 1.0f,  -1.0f,		0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//22: G
		0.375f,  1.0f,  -1.0f,		0.0f,	0.143,		0.0f,	-1.0f,	0.0f,	//23: H

	};

	Mesh* agua = new Mesh();
	agua->CreateMesh(agua_vertices, agua_indices, 192, 36);
	meshList.push_back(agua);
}

//Funci�n crea primitiva: arena
void CrearArena()
{
	unsigned int arena_indices[] = {
		//Front
		0, 1, 2,
		1, 2, 3,
		//Back
		4, 5, 6,
		5, 6, 7,
		//Left
		8, 9, 10,
		9, 10, 11,
		//Right
		12, 13, 14,
		13, 14, 15,
		//Bottom
		16, 17, 18,
		17, 18, 19,
		//Top
		20, 21, 22,
		21, 22, 23,
	};


	GLfloat arena_vertices[] = {
		//Front
		//x			y		z		S		T			-NX		-NY		-NZ
		0.125f,	 0.0f,  0.0f,		0.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//0: B
		1.0f,	 0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//1: I
		0.375f,	 1.0f,  0.0f,		0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2: D
		1.0f,	 1.0f,  0.0f,		1.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3: J

		//Back
		//x		y		z			S		T			-NX		-NY		-NZ
		0.125f,  0.0f,  -1.0f,		0.0f,	0.0f,		0.0f,	0.0f,	1.0f,	//5: F
		1.0f,	 0.0f,  -1.0f,		1.0f,   0.0f,		0.0f,	0.0f,	1.0f,	//4: K
		0.375f,  1.0f,  -1.0f,		0.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//7: H
		1.0f,	 1.0f,  -1.0f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	//6: L

		//Left
		//x		y		z			S		T			-NX		-NY		-NZ
		0.125f,	 0.0f,  0.0f,		0.0f,	0.0f,		1.0f,	0.0f,	0.0f,	//8: B
		0.125f,  0.0f,  -1.0f,		1.0f,   0.0f,		1.0f,	0.0f,	0.0f,	//9: F
		0.375f,	 1.0f,  0.0f,		0.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//10: D
		0.375f,  1.0f,  -1.0f,		1.0f,	1.0f,		1.0f,	0.0f,	0.0f,	//11: H

		//Right
		//x		y		z			S		T			-NX		-NY		-NZ
		1.0f,	 0.0f,  0.0f,		0.0f,	0.0f,		-1.0f,	0.0f,	0.0f,	//12: I
		1.0f,	 0.0f,  -1.0f,		1.0f,   0.0f,		-1.0f,	0.0f,	0.0f,	//13: K
		1.0f,	 1.0f,  0.0f,		0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//14: J
		1.0f,	 1.0f,  -1.0f,		1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	//15: L

		//Bottom
		//x		y		z			S		T			-NX		-NY		-NZ
		0.125f,	 0.0f,  0.0f,		0.0f,	0.0f,		0.0f,	1.0f,	0.0f,	//16: B
		1.0f,	 0.0f,  0.0f,		1.0f,   0.0f,		0.0f,	1.0f,	0.0f,	//17: I
		0.125f,  0.0f,  -1.0f,		0.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//18: F
		1.0f,	 0.0f,  -1.0f,		1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	//19: K

		//Top
		//x		y		z			S		T			-NX		-NY		-NZ
		0.375f,	 1.0f,  0.0f,		0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//20: D
		1.0f,	 1.0f,  0.0f,		1.0f,   0.0f,		0.0f,	-1.0f,	0.0f,	//21: J
		0.375f,  1.0f,  -1.0f,		0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//22: H
		1.0f,	 1.0f,  -1.0f,		1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	//23: L

	};

	Mesh* arena = new Mesh();
	arena->CreateMesh(arena_vertices, arena_indices, 192, 36);
	meshList.push_back(arena);
}

//Funci�n crea primitiva: casa

void CrearCasa()
{
	unsigned int casa_indices[] = {
		//Front
		0, 1, 2,
		1, 2, 3,
		2, 5, 6,
		2, 3, 6,
		4, 5, 6,

		//Back
		7, 8, 9,
		8, 9, 10,
		9, 12, 13,
		9, 10, 13,
		11, 12, 13,

		//Right
		20, 21, 18,
		21, 19, 18,
		18, 19, 16,
		19, 17, 16,
		16, 17, 14,
		17, 15, 14,

		//Left
		28, 29, 26,
		29, 27, 26,
		26, 27, 24,
		27, 25, 24,
		24, 25, 22,
		25, 23, 22,

		//Bottom
		30, 31, 32,
		31, 32, 33,
	};


	GLfloat casa_vertices[] = {
		//Front
		//x					y					z			S					T					-NX	  -NY	 -NZ
		0.087235487642f,	0.0011752714266f,	0.0f,		0.0479565399097f,	0.49932519425395f,   0.0f, 0.0f, -1.0f,     //0: C
		0.8660852434438f,	0.0022203162877f,	0.0f,		0.4373814178106f,	0.4998477166845f,    0.0f, 0.0f, -1.0f,     //1: D
		0.0482761515455f,	0.4169979723985f,	0.0f,		0.02847687186145f,	0.7072365447399f,    0.0f, 0.0f, -1.0f,     //2: E
		0.9117380930899f,	0.4144286735813f,	0.0f,		0.46020784263365f,	0.7059518953313f,    0.0f, 0.0f, -1.0f,     //3: F
		0.4815969563616f,	1.0025248829187f,	0.0f,		0.2451372742695f,	1.0f,				 0.0f, 0.0f, -1.0f,     //4: G
		0.0015197330629f,	0.4848379232325f,	0.0f,		0.00509866262015f,	0.7411565201569f,    0.0f, 0.0f, -1.0f,     //5: H
		0.971549737457f,	0.4709889866024f,	0.0f,		0.4901136648172f,	0.73423205184185f,   0.0f, 0.0f, -1.0f,     //6: I

		//Back
		//x					y					z			S					T				-NX	  -NY	 -NZ
		0.087235487642f,	0.0011752714266f,	-1.0f,		0.04795654f,	0.002301136f,		0.0f, 0.0f, 1.0f,     //7: C1
		0.8660852434438f,	0.0022203162877f,	-1.0f,		0.437381418f,	0.002823659f,		0.0f, 0.0f, 1.0f,     //8: D1
		0.0482761515455f,	0.4169979723985f,	-1.0f,		0.028476872f,	0.210212487f,		0.0f, 0.0f, 1.0f,     //9: E1
		0.9117380930899f,	0.4144286735813f,	-1.0f,		0.460207843f,	0.208927837f,		0.0f, 0.0f, 1.0f,     //10: F1
		0.4815969563616f,	1.0025248829187f,	-1.0f,		0.245137274f,	0.502975942f,		0.0f, 0.0f, 1.0f,     //11: G1
		0.0015197330629f,	0.4848379232325f,	-1.0f,		0.005098663f,	0.244132462f,		0.0f, 0.0f, 1.0f,     //12: H1
		0.971549737457f,	0.4709889866024f,	-1.0f,		0.490113665f,	0.237207994f,		0.0f, 0.0f, 1.0f,     //13: I1

		//Right 
		//x					y					z			S		T			-NX	  -NY	 -NZ
		0.4815969563616f,	1.0025248829187f,	0.0f,		0.5f,	0.5f,		-1.0f, 0.0f, 0.0f,     //14: G
		0.4815969563616f,	1.0025248829187f,	-1.0f,		1.0f,	0.5f,		-1.0f, 0.0f, 0.0f,     //15: G1
		0.971549737457f,	0.4709889866024f,	0.0f,		0.5f,	0.3033f,    -1.0f, 0.0f, 0.0f,     //16: I
		0.971549737457f,	0.4709889866024f,	-1.0f,		1.0f,	0.3033f,	-1.0f, 0.0f, 0.0f,     //17: I1
		0.9117380930899f,	0.4144286735813f,	0.0f,		0.5f,	0.25f,		-1.0f, 0.0f, 0.0f,     //18: F
		0.9117380930899f,	0.4144286735813f,	-1.0f,		1.0f,	0.25f,		-1.0f, 0.0f, 0.0f,     //19: F1
		0.8660852434438f,	0.0022203162877f,	0.0f,		0.5f,	0.0f,		-1.0f, 0.0f, 0.0f,     //20: D
		0.8660852434438f,	0.0022203162877f,	-1.0f,		1.0f,	0.0f,		-1.0f, 0.0f, 0.0f,     //21: D1

		//Left
		//x					y					z			S		T			-NX	  -NY	 -NZ
		0.4815969563616f,	1.0025248829187f,	0.0f,		0.5f,	0.5f,		1.0f, 0.0f, 0.0f,     //22: G
		0.4815969563616f,	1.0025248829187f,	-1.0f,		1.0f,	0.5f,		1.0f, 0.0f, 0.0f,     //23: G1
		0.0015197330629f,	0.4848379232325f,	0.0f,		0.5f,	0.3033f,    1.0f, 0.0f, 0.0f,     //24: H
		0.0015197330629f,	0.4848379232325f,	-1.0f,		1.0f,	0.3033f,	1.0f, 0.0f, 0.0f,     //25: H1
		0.0482761515455f,	0.4169979723985f,	0.0f,		0.5f,	0.25f,		1.0f, 0.0f, 0.0f,     //26: E1
		0.0482761515455f,	0.4169979723985f,	-1.0f,		1.0f,	0.25f,		1.0f, 0.0f, 0.0f,     //27: E1
		0.087235487642f,	0.0011752714266f,	0.0f,		0.5f,	0.0f,		1.0f, 0.0f, 0.0f,     //28: C
		0.087235487642f,	0.0011752714266f,	-1.0f,		1.0f,	0.0f,		1.0f, 0.0f, 0.0f,     //29: C1

		//Bottom
		0.8660852434438f,	0.0022203162877f,	0.0f,		0.1f,	0.1f,		0.0f, 1.0f, 0.0f,     //30: D
		0.8660852434438f,	0.0022203162877f,	-1.0f,		0.2f,	0.1f,		0.0f, 1.0f, 0.0f,     //31: D1
		0.087235487642f,	0.0011752714266f,	0.0f,		0.1f,	0.2f,		0.0f, 1.0f, 0.0f,     //32: C
		0.087235487642f,	0.0011752714266f,	-1.0f,		0.2f,	0.2f,		0.0f, 1.0f, 0.0f,     //33: C1

	};


	Mesh* casa = new Mesh();
	casa->CreateMesh(casa_vertices, casa_indices, 272, 72);
	meshList.push_back(casa);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}





///////////////////////////////KEYFRAMES/////////////////////  

bool animacion = false;


//NEW// Keyframes
float posXAnimacion = 30.0, posYAnimacion = 0.0, posZAnimacion = -60.0;
float	movAnimacion_x = 0.0f, movAnimacion_y = 0.0f, movAnimacion_z = 0.0f;
float giroAnimacion = 0;
float giroDosAnimacion = 0;
float giroTresAnimacion = 0;


int i_max_steps = 90;
int i_curr_steps = 1;  //igual a FrameIndex siempre
int FrameIndex = 1;			//introducir datos
bool play = false;
int playIndex = 0;

FRAME KeyFrame[MAX_FRAMES];



//se puede hacer DELTATIME dentro de la interpolacion o en la reproduccion
void saveFrame(void)
{

	KeyFrame[FrameIndex].movAnimacion_x = movAnimacion_x;
	KeyFrame[FrameIndex].movAnimacion_y = movAnimacion_y;
	KeyFrame[FrameIndex].movAnimacion_z = movAnimacion_z;
	KeyFrame[FrameIndex].giroAnimacion = giroAnimacion;
	KeyFrame[FrameIndex].giroDosAnimacion = giroDosAnimacion;
	KeyFrame[FrameIndex].giroTresAnimacion = giroTresAnimacion;


	//Keyframes ingresados mostrados en consola
	printf("KeyFrame[%d].movAnimacion_x = %f\n", FrameIndex, movAnimacion_x);
	printf("KeyFrame[%d].movAnimacion_y = %f\n", FrameIndex, movAnimacion_y);
	printf("KeyFrame[%d].movAnimacion_z = %f\n", FrameIndex, movAnimacion_z);
	printf("KeyFrame[%d].giroAnimacion = %f\n", FrameIndex, giroAnimacion);
	printf("KeyFrame[%d].giroDosAnimacion = %f\n", FrameIndex, giroDosAnimacion);
	printf("KeyFrame[%d].giroTresAnimacion = %f\n", FrameIndex, giroTresAnimacion);
	printf("\n");

	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].movAnimacion_x = " + std::to_string(movAnimacion_x));
	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].movAnimacion_y = " + std::to_string(movAnimacion_y));
	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].movAnimacion_z = " + std::to_string(movAnimacion_z));
	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].giroAnimacion = " + std::to_string(giroAnimacion));
	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].giroDosAnimacion = " + std::to_string(giroDosAnimacion));
	archivo1.LeerKeyFrames("KeyFrame[" + std::to_string(FrameIndex) + "].giroTresAnimacion = " + std::to_string(giroTresAnimacion));


	FrameIndex++;
}

void resetElements(void)
{

	movAnimacion_x = KeyFrame[0].movAnimacion_x;
	movAnimacion_y = KeyFrame[0].movAnimacion_y;
	movAnimacion_z = KeyFrame[0].movAnimacion_z;
	giroAnimacion = KeyFrame[0].giroAnimacion;
	giroDosAnimacion = KeyFrame[0].giroDosAnimacion;
	giroTresAnimacion = KeyFrame[0].giroTresAnimacion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAnimacion_xInc = (KeyFrame[playIndex + 1].movAnimacion_x - KeyFrame[playIndex].movAnimacion_x) / i_max_steps;
	KeyFrame[playIndex].movAnimacion_yInc = (KeyFrame[playIndex + 1].movAnimacion_y - KeyFrame[playIndex].movAnimacion_y) / i_max_steps;
	KeyFrame[playIndex].movAnimacion_zInc = (KeyFrame[playIndex + 1].movAnimacion_z - KeyFrame[playIndex].movAnimacion_z) / i_max_steps;
	KeyFrame[playIndex].giroAnimacionInc = (KeyFrame[playIndex + 1].giroAnimacion - KeyFrame[playIndex].giroAnimacion) / i_max_steps;
	KeyFrame[playIndex].giroDosAnimacionInc = (KeyFrame[playIndex + 1].giroDosAnimacion - KeyFrame[playIndex].giroDosAnimacion) / i_max_steps;
	KeyFrame[playIndex].giroTresAnimacionInc = (KeyFrame[playIndex + 1].giroTresAnimacion - KeyFrame[playIndex].giroTresAnimacion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			movAnimacion_x += KeyFrame[playIndex].movAnimacion_xInc;
			movAnimacion_y += KeyFrame[playIndex].movAnimacion_yInc;
			movAnimacion_z += KeyFrame[playIndex].movAnimacion_zInc;
			giroAnimacion += KeyFrame[playIndex].giroAnimacionInc;
			giroDosAnimacion += KeyFrame[playIndex].giroDosAnimacionInc;
			giroTresAnimacion += KeyFrame[playIndex].giroTresAnimacionInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



//  === MAIN ===
int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();				//mesh[0 a 5]
	CrearDado();					//mesh[6]
	CreateShaders();
	CrearCubo();					//mesh[7]
	CrearPiramideCuadrangular();	//mesh[8]

	CrearMesa();					//mesh[9]
	CrearAgua();					//mesh[10]
	CrearArena();					//mesh[11]
	CrearCasa();					//mesh[12]

	camera = Camera(glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	// === Texturas ===
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pisoAzul.tga");
	pisoTexture.LoadTextureA();

	texto = Texture("Textures/textoOpen.tga");
	texto.LoadTextureA();
	dadoTexture = Texture("Textures/dado8.jpg");
	//dadoTexture.LoadTextureA();
	dadoTexture.LoadTexture();

	mesaTexture = Texture("Textures/Wood.tga");
	mesaTexture.LoadTexture();
	aguaTexture = Texture("Textures/mar.png");
	aguaTexture.LoadTextureA();
	arenaTexture = Texture("Textures/Arena.tga");
	arenaTexture.LoadTexture();
	casaTexture = Texture("Textures/Casa.tga");
	casaTexture.LoadTextureA();

	LeviathanTexture = Texture("Textures/Reaper_Leviathan.png");
	LeviathanTexture.LoadTextureA();
	PezRojoTexture = Texture("Textures/PezRojo.png");
	PezRojoTexture.LoadTextureA();
	PenglingTexture = Texture("Textures/Pengling.png");
	PenglingTexture.LoadTextureA();
	CapsulasTexture = Texture("Textures/Capsulas.png");
	CapsulasTexture.LoadTextureA();
	UmbrellaTexture = Texture("Textures/Umbrella.png");
	UmbrellaTexture.LoadTextureA();
	NetTexture = Texture("Models/Net.png");
	NetTexture.LoadTextureA();

	// === Carga de modelos ===

	//Faro
	faro = Model();
	faro.LoadModel("Models/Pruebas/Street Lamp.obj");
	antorcha = Model();
	antorcha.LoadModel("Models/Pruebas/Torch.obj");

	//Papalote
	papalote = Model();
	papalote.LoadModel("Models/kite sf.obj");

	//Pingu
	Pingu = Model();
	Pingu.LoadModel("Models/Pingu.obj");

	//Tom Nook
	tomNook = Model();
	tomNook.LoadModel("Models/tomNook.obj");

	//Bolsa de dinero
	bolsaDinero = Model();
	bolsaDinero.LoadModel("Models/monedero.obj");

	//Leviathan
	Leviathan_M = Model();
	Leviathan_M.LoadModel("Models/Leviathan.obj");

	//Pez
	PezRojo_M = Model();
	PezRojo_M.LoadModel("Models/PezRojo.obj");

	//Robin:Avatar
	BodyRobin = Model();
	BodyRobin.LoadModel("Models/RobinCuerpo.obj");
	LegLeftRobin = Model();
	LegLeftRobin.LoadModel("Models/RobinPiernaIzquierda.obj");
	LegRightRobin = Model();
	LegRightRobin.LoadModel("Models/RobinPiernaDerecha.obj");
	ArmLeftRobin = Model();
	ArmLeftRobin.LoadModel("Models/RobinBrazoIzquierdo.obj");
	ArmRightRobin = Model();
	ArmRightRobin.LoadModel("Models/RobinBrazoDerecho.obj");

	//Pengling
	Pengling_M = Model();
	Pengling_M.LoadModel("Models/Pengling.obj");

	//Capsulas
	Capsulas_M = Model();
	Capsulas_M.LoadModel("Models/Capsulas.obj");

	//Sombrilla
	Umbrella_M = Model();
	Umbrella_M.LoadModel("Models/Umbrella.obj");

	//Silla de playa
	BeachChair_M = Model();
	BeachChair_M.LoadModel("Models/BeachChair.obj");

	//Red
	Net_M = Model();
	Net_M.LoadModel("Models/Net.obj");

	//Dron
	dron = Model();
	dron.LoadModel("Models/dronSinhelices.obj");

	heliceDron = Model();
	heliceDron.LoadModel("Models/cuatroHelices.obj");

	// === Skybox ===
	std::vector<std::string> skyboxFaces;
	//Day
	skyboxFaces.push_back("Textures/Skybox/Day_Right.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Left.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Top.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Front.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Back.tga");
	skybox = Skybox(skyboxFaces);
	//Night
	std::vector<std::string> skyboxFaces_night;
	skyboxFaces_night.push_back("Textures/Skybox/Night_Right.tga");
	skyboxFaces_night.push_back("Textures/Skybox/Night_Left.tga");
	skyboxFaces_night.push_back("Textures/Skybox/Night_Bottom.tga");
	skyboxFaces_night.push_back("Textures/Skybox/Night_Top.tga");
	skyboxFaces_night.push_back("Textures/Skybox/Night_Front.tga");
	skyboxFaces_night.push_back("Textures/Skybox/Night_Back.tga");

	skybox_night = Skybox(skyboxFaces_night);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	// === Luz Direccional === 
	//Se implementan 5 etapas de día y 5 etapas de sol, cada una con su propia intensidad ambiental y dirección
	//Definición etapas ciclo día
	mainLight_1 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.4f,
		0.0f, 0.0f, -1.0f);
	mainLight_2 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.45f, 0.4f,
		0.0f, -1.0f, -1.0f);
	mainLight_3 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.4f,
		0.0f, -1.0f, 0.0f);
	mainLight_4 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.45f, 0.4f,
		0.0f, -1.0f, 1.0f);
	mainLight_5 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.4f,
		0.0f, -1.0f, 1.0f);

	//Definicion etapas ciclo noche
	mainLight_night_1 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.4f,
		0.0f, 0.0f, -1.0f);
	mainLight_night_2 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.15f, 0.4f,
		0.0f, -1.0f, -1.0f);
	mainLight_night_3 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.2f, 0.4f,
		0.0f, -1.0f, 0.0f);
	mainLight_night_4 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.15f, 0.4f,
		0.0f, -1.0f, 1.0f);
	mainLight_night_5 = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.4f,
		0.0f, 0.0f, 1.0f);


	// === Luz Puntal ===
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		30.0f, 3.5f, 50.0f,
		1.0f, 0.03f, 0.006f);
	//0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		30.0f, 5.5f, 165.0f,
		1.0f, 0.01f, 0.001f);
	//0.3f, 0.2f, 0.1f);
	pointLightCount++;


	// === Luz Reflector ===
	unsigned int spotLightCount = 0;
	////linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;

	////luz fija
	//spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;

	//1� Luz [Reflector]: Luz frontal del coche
	spotLightsInverso[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//2� Luz [Reflector]: Luz trasera del coche
	spotLightsInverso[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//3� Luz [Reflector]: Luz del helicoptero
	spotLightsInverso[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);

	//Variables Uniform
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	//Perspectiva
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	//KeyFrames iniciales
	KeyFrame[0].movAnimacion_x = 0.0f;
	KeyFrame[0].movAnimacion_y = 0.0f;
	KeyFrame[0].movAnimacion_z = 0.0f;
	KeyFrame[0].giroAnimacion = 0;
	KeyFrame[0].giroDosAnimacion = 0;
	KeyFrame[0].giroTresAnimacion = 0;

	//Se leen los keyFrames del archivo
	archivo1.LeerKeyFramesDeArchivo();


	/*int index = 0;
	int size_of_KeyFrame = 10;
	while (index <= size_of_KeyFrame) {
		printf("KeyFrame[%d].movpapalote_x = %f\n", index, KeyFrame[index].movAnimacion_x);
		printf("KeyFrame[%d].movpapalote_y = %f\n", index, KeyFrame[index].movAnimacion_y);
		printf("KeyFrame[%d].movpapalote_z = %f\n", index, KeyFrame[index].movAnimacion_z);
		printf("KeyFrame[%d].giropapalote = %f\n", index, KeyFrame[index].giroAnimacion);
		printf("KeyFrame[%d].giroDospapalote = %f\n", index, KeyFrame[index].giroDosAnimacion);
		printf("KeyFrame[%d].giroTrespapalote = %f\n", index, KeyFrame[index].giroTresAnimacion);
		index++;
	}*/


	//Inicializaci�n variables animaci�n

	//Movimiento pengling
	offsetAvanzaPengling = 0.4f;
	offsetGiroPengling = 3.0f;
	penglingOffset = 5.0f;  //es el senoidal
	bool adelantePengling = true;
	activarAnimacionPengling = false;

	//Movimiento Tom Nook
	offsetAvanzaTomNook = 0.3f;
	offsetGiroTomNook = 3.0f;
	tomNookOffset = 5.0f;  //es el senoidal
	bool adelanteTomNook = true;
	activarAnimacionTomNook = false;

	//Movimiento de bolsa de dinero
	offsetGiroBolsa = 4;
	rotacionBolsaEje = 0.0;
	offsetBolsaArribaAbajo = 4.0f;
	movBolsaArribaAbajo = 0.0f;
	activarAnimacionBolsa = false;

	//Movimiento de salto
	offsetAvanzaPez = 0.4f;
	offsetGiroPez = 3.0f;
	pezOffset = 5.0f;  //es el senoidal
	offsetPezArribaAbajo = 4.0f;
	movPezX = 0.0f;
	movPezZ = 0.0f;
	movPezY = 270.0f;
	bool adelantePez = true;
	float compSeno = -1.0f;

	//Movimiento de Leviathan
	offsetAvanzaLev = 0.4;
	offsetGiroLev = 0.5f;
	levOffset = 0.35f;
	movLevX = 0;
	movLevY = 0;
	movLevZ = 0.0f;
	rotaLev = 0;
	float resultado = 0;
	bool controlDireccionLev = false;

	//Movimiento circular
	offsetAvanzaCirculo = 0.7;
	offsetGiroCirculo = 0.5f;
	circuloOffset = 0.35f;
	movCirculoX = 0;
	radioCirculo = 60.0f;
	movCirculoZ = 0.0f;
	anguloCirculo = 0;

	//variables ciclo día-noche
	float startTime = 0.0f;
	float nowTime;
	float diaNoche = 0.0f; //O: día, 1:noche
	float duracionDia = 30.0f;
	float duracionDia_1 = duracionDia / 5;
	float duracionDia_2 = (duracionDia / 5) * 2;
	float duracionDia_3 = (duracionDia / 5) * 3;
	float duracionDia_4 = (duracionDia / 5) * 4;
	float contTime = 0.0f;

	//Letrero de la casa de tom Nook
	glm::vec3 posicionLetreroCasaTomNook = glm::vec3(40.0f, 0.0f, -135.0);

	//parametros de Arquimedes
	// Variables para la animaci�n en espiral de Arqu�medes
	float t = 0.0f;                     // Par�metro t de la espiral
	float spiralRadius = 2.0f;          // Radio de la espiral
	float spiralHeight = 0.1f;         // Altura de la espiral
	float spiralSpeed = 0.15f;           // Velocidad de la animaci�n

	//numero random
	// Providing a seed value
	srand((unsigned)time(NULL));



	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		nowTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Ciclo dia y noche: cambio de dia y de noche:  diaNoche=0.0f para día, diaNoche=1.0f para noche
		contTime = nowTime - startTime;
		if (contTime >= duracionDia) {
			startTime = nowTime;
			if (diaNoche == 0.0f)diaNoche = 1.0f;
			else diaNoche = 0.0f;
		}
		if (diaNoche == 0.0f) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection); //skyboxdia
		}
		else {
			skybox_night.DrawSkybox(camera.calculateViewMatrix(), projection); //skybox de noche
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		uniformTextureOffset = shaderList[0].getOffsetLocation();


		//Informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		//=== luz ligada c�mara [flash] ===
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//Ciclo dia noche junto con encendido y apagado de luces tipo pointlight
		if (diaNoche == 1.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			if (contTime < duracionDia_1)shaderList[0].SetDirectionalLight(&mainLight_night_1);
			else if (contTime < duracionDia_2)shaderList[0].SetDirectionalLight(&mainLight_night_2);
			else if (contTime < duracionDia_3)shaderList[0].SetDirectionalLight(&mainLight_night_3);
			else if (contTime < duracionDia_4)shaderList[0].SetDirectionalLight(&mainLight_night_4);
			else if (contTime < duracionDia)shaderList[0].SetDirectionalLight(&mainLight_night_5);
		}
		else {
			shaderList[0].SetPointLights(pointLights, 0);
			//shaderList[0].SetDirectionalLight(&mainLight);
			if (contTime < duracionDia_1)shaderList[0].SetDirectionalLight(&mainLight_1);
			else if (contTime < duracionDia_2)shaderList[0].SetDirectionalLight(&mainLight_2);
			else if (contTime < duracionDia_3)shaderList[0].SetDirectionalLight(&mainLight_3);
			else if (contTime < duracionDia_4)shaderList[0].SetDirectionalLight(&mainLight_4);
			else if (contTime < duracionDia)shaderList[0].SetDirectionalLight(&mainLight_5);
		}
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Ciclo dia noche

		//=== Modelos ===
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxHelice(1.0);
		glm::mat4 modelauxRobin(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		//=== Antorcha ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 2.0f, 165.0));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		antorcha.RenderModel();


		// ========= PROYECTO =========

		//=== Primitiva: Mesa ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 2.0f, 165.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesaTexture.UseTexture();
		meshList[9]->RenderMesh();

		//=== Primitiva: Arena ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-300.0f, -102.0f, 300.0));		//x:- (EscalaX/2)  y:- (EscalaY+2)  z: EscalaZ/2
		model = glm::scale(model, glm::vec3(600.0f, 100.0f, 600.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arenaTexture.UseTexture();
		meshList[11]->RenderMesh();

		//=== Primitiva: Casa ===
		model = glm::mat4(1.0);
		model = glm::translate(model, posicionLetreroCasaTomNook);
		model = glm::scale(model, glm::vec3(72.0f, 72.0f, 72.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez  menor al 100%
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaTexture.UseTexture();
		meshList[12]->RenderMesh();
		glDisable(GL_BLEND);


		//=== Pengling: Modelo de pinguino ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f + movPenglingX, -2.0f, 200.0 + movPenglingZ));
		model = glm::scale(model, glm::vec3(5.9f, 5.9f, 5.9f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (movPenglingZ > 50) {
			movPenglingZ = 49;
		}

		//rotacion En circuito
		model = glm::rotate(model, rotaPengling * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		if (activarAnimacionPengling == true) {
			//Movimiento del pinguino
			if (adelantePengling == true) {
				if (movPenglingZ < 25.0f)
				{
					movPenglingZ += offsetAvanzaPengling * deltaTime;
				}
				else if (movPenglingZ >= 25.0 and movPenglingZ < 28.0f) {
					movPenglingZ += 0.1 * deltaTime;
					rotaPengling += offsetGiroPengling * deltaTime;
				}
				else if (movPenglingX < 30.0f) {
					movPenglingX += offsetAvanzaPengling * deltaTime;
				}
				else if (movPenglingX >= 30.0 and movPenglingX < 33.0f) {
					movPenglingX += 0.1 * deltaTime;
					rotaPengling += offsetGiroPengling * deltaTime;
				}
				else {
					adelantePengling = false;
				}

			}
			else {
				if (movPenglingZ > -25.0f)
				{
					movPenglingZ -= offsetAvanzaPengling * deltaTime;
				}
				else if (movPenglingZ <= -25.0 and movPenglingZ > -28.0f) {
					movPenglingZ -= 0.1 * deltaTime;
					rotaPengling += offsetGiroPengling * deltaTime;
				}
				else if (movPenglingX > -20.0f) {
					movPenglingX -= offsetAvanzaPengling * deltaTime;
				}
				else if (movPenglingX <= -20.0 and movPenglingX > -23.0f) {
					movPenglingX -= 0.1 * deltaTime;
					rotaPengling += offsetGiroPengling * deltaTime;
				}
				else {
					adelantePengling = true;
				}
			}
			if (movPenglingZ > 359.0) {
				penglingOffset = 0.0f;
			}
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PenglingTexture.UseTexture();
		Pengling_M.RenderModel();


		//=== Pingu: Modelo de pinguino : Animación de circulo ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f + movCirculoX, 0.0f, 18.0f + movCirculoZ));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 350 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -anguloCirculo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		movCirculoX = radioCirculo * cos(glm::radians(anguloCirculo));
		movCirculoZ = radioCirculo * sin(glm::radians(anguloCirculo));
		anguloCirculo += offsetAvanzaCirculo * deltaTime;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pingu.RenderModel();


		////=== Robin: Subnautica Below Zero, Avatar === SE MUEVE CON LA CAMARA
		mainWindow.setcameraMode(camera.getcameraMode());
		if (mainWindow.getmovAvatar() == 1.0f) {
			switch (mainWindow.getsentido())
			{
			case 0:		//movimiento hacia adelante
				if (contadorRobin < 20) {
					if (auxiliarMovimiento > 0.0f) {
						if (piernaMovRobin == 1.0f) {
							rotaBrazoDerRobin += offsetRotaBrazoRobin * deltaTime;
							rotaPiernaDerRobin -= offsetRotaPiernaRobin * deltaTime;
							rotaBrazoIzqRobin -= offsetRotaBrazoRobin * deltaTime;
							rotaPiernaIzqRobin += offsetRotaPiernaRobin * deltaTime;
						}
						else {
							rotaBrazoDerRobin -= offsetRotaBrazoRobin * deltaTime;
							rotaPiernaDerRobin += offsetRotaPiernaRobin * deltaTime;
							rotaBrazoIzqRobin += offsetRotaBrazoRobin * deltaTime;
							rotaPiernaIzqRobin -= offsetRotaPiernaRobin * deltaTime;
						}

					}
					else {

						if (auxiliarMovimiento == 0.0f) {
							if (piernaMovRobin > 0.0f) {
								rotaBrazoDerRobin -= offsetRotaBrazoRobin * deltaTime;
								rotaPiernaDerRobin += offsetRotaPiernaRobin * deltaTime;
								rotaBrazoIzqRobin += offsetRotaBrazoRobin * deltaTime;
								rotaPiernaIzqRobin -= offsetRotaPiernaRobin * deltaTime;
							}
							else {
								rotaBrazoDerRobin += offsetRotaBrazoRobin * deltaTime;
								rotaPiernaDerRobin -= offsetRotaPiernaRobin * deltaTime;
								rotaBrazoIzqRobin -= offsetRotaBrazoRobin * deltaTime;
								rotaPiernaIzqRobin += offsetRotaPiernaRobin * deltaTime;
							}
						}
					}
					contadorRobin++;
					if (contadorRobin == 10) {
						if (auxiliarMovimiento == 1.0f)auxiliarMovimiento = 0.0f;
						else auxiliarMovimiento = 1.0f;
					}
				}
				else {
					contadorRobin = 0;
					mainWindow.setmovAvatar_r(0.0f);
					mainWindow.setmovAvatar(0.0f);
				}
				break;
			case 1:		//movimiento hacia atras
				if (contadorRobin < 20) {
					if (auxiliarMovimiento > 0.0f) {
						if (piernaMovRobin == 1.0f) {
							rotaBrazoDerRobin += offsetRotaBrazoRobin * deltaTime;
							rotaPiernaDerRobin -= offsetRotaPiernaRobin * deltaTime;
							rotaBrazoIzqRobin -= offsetRotaBrazoRobin * deltaTime;
							rotaPiernaIzqRobin += offsetRotaPiernaRobin * deltaTime;
						}
						else {
							rotaBrazoDerRobin -= offsetRotaBrazoRobin * deltaTime;
							rotaPiernaDerRobin += offsetRotaPiernaRobin * deltaTime;
							rotaBrazoIzqRobin += offsetRotaBrazoRobin * deltaTime;
							rotaPiernaIzqRobin -= offsetRotaPiernaRobin * deltaTime;
						}

					}
					else {

						if (auxiliarMovimiento == 0.0f) {
							if (piernaMovRobin > 0.0f) {
								rotaBrazoDerRobin -= offsetRotaBrazoRobin * deltaTime;
								rotaPiernaDerRobin += offsetRotaPiernaRobin * deltaTime;
								rotaBrazoIzqRobin += offsetRotaBrazoRobin * deltaTime;
								rotaPiernaIzqRobin -= offsetRotaPiernaRobin * deltaTime;
							}
							else {
								rotaBrazoDerRobin += offsetRotaBrazoRobin * deltaTime;
								rotaPiernaDerRobin -= offsetRotaPiernaRobin * deltaTime;
								rotaBrazoIzqRobin -= offsetRotaBrazoRobin * deltaTime;
								rotaPiernaIzqRobin += offsetRotaPiernaRobin * deltaTime;
							}
						}
					}
					contadorRobin++;
					if (contadorRobin == 10) {
						if (auxiliarMovimiento == 1.0f)auxiliarMovimiento = 0.0f;
						else auxiliarMovimiento = 1.0f;
					}
				}
				else {
					contadorRobin = 0;
					mainWindow.setmovAvatar_r(0.0f);
					mainWindow.setmovAvatar(0.0f);
				}
				break;
			case 2:		//movimiento hacia la izquierda
				if (contadorRobin < 30) {
					if (aperturaRobin == 0.0f) {
						rotaPiernaXDerRobin += offsetRotaXPiernaRobin * deltaTime;
						rotaPiernaXIzqRobin -= offsetRotaXPiernaRobin * deltaTime;
					}
					else {
						rotaPiernaXDerRobin -= offsetRotaXPiernaRobin * deltaTime;
						rotaPiernaXIzqRobin += offsetRotaXPiernaRobin * deltaTime;
					}
					contadorRobin++;
					if (contadorRobin == 15) {
						if (aperturaRobin == 1.0f)aperturaRobin = 0.0f;
						else aperturaRobin = 1.0f;
					}
				}
				else {
					contadorRobin = 0;
					aperturaRobin = 0.0f;
					mainWindow.setmovAvatar_r(0.0f);
					mainWindow.setmovAvatar(0.0f);
				}
				break;
			case 3:		//movimiento hacia la derecha
				if (contadorRobin < 30) {
					if (aperturaRobin == 0.0f) {
						rotaPiernaXDerRobin += offsetRotaXPiernaRobin * deltaTime;
						rotaPiernaXIzqRobin -= offsetRotaXPiernaRobin * deltaTime;
					}
					else {
						rotaPiernaXDerRobin -= offsetRotaXPiernaRobin * deltaTime;
						rotaPiernaXIzqRobin += offsetRotaXPiernaRobin * deltaTime;
					}
					contadorRobin++;
					if (contadorRobin == 15) {
						if (aperturaRobin == 1.0f)aperturaRobin = 0.0f;
						else aperturaRobin = 1.0f;
					}
				}
				else {
					contadorRobin = 0;
					aperturaRobin = 0.0f;
					mainWindow.setmovAvatar_r(0.0f);
					mainWindow.setmovAvatar(0.0f);
				}
				break;
			default:
				break;
			}

		}


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, 0.0));
		model = glm::translate(model, glm::vec3(posicionPersonaje));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -anguloPersonaje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxRobin = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BodyRobin.RenderModel();
		//Pierna izquierda
		model = modelauxRobin;
		model = glm::translate(model, glm::vec3(-0.1f, -0.3f, 0.21));
		model = glm::rotate(model, rotaPiernaIzqRobin * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotaPiernaXIzqRobin * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LegLeftRobin.RenderModel();
		//Pierna derecha
		model = modelauxRobin;
		model = glm::translate(model, glm::vec3(-0.1f, -0.3f, -0.21));
		model = glm::rotate(model, rotaPiernaDerRobin * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotaPiernaXDerRobin * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LegRightRobin.RenderModel();
		//Brazo izquierdo
		model = modelauxRobin;
		model = glm::translate(model, glm::vec3(-0.1f, 0.9f, -0.21));
		model = glm::rotate(model, rotaBrazoIzqRobin * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArmLeftRobin.RenderModel();
		//Brazo derecho
		model = modelauxRobin;
		model = glm::translate(model, glm::vec3(-0.1f, 0.9f, 0.21));
		model = glm::rotate(model, rotaBrazoDerRobin * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArmRightRobin.RenderModel();

		//=== Tom Nook: Modelo de mapache
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f + movTomNookX, -2.0f, -100.0));
		model = glm::scale(model, glm::vec3(6.7f, 6.7f, 6.7f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//Lineas que hacebn que no se inicialice desde coordenadas muy lejanas
		if (movTomNookX > 10.0f) {
			movTomNookX = 0.0f;
		}

		model = glm::rotate(model, rotaTomNook * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		if (activarAnimacionTomNook == true) {
			//Movimiento de Tom Nook
			if (adelanteTomNook == true) {
				if (movTomNookX < 4.0f)
				{
					movTomNookX += offsetAvanzaTomNook * deltaTime;
					rotaTomNook += offsetGiroTomNook * deltaTime;
				}
				else {
					adelanteTomNook = false;
				}
			}
			else {
				if (movTomNookX > -4.0f)
				{
					movTomNookX -= offsetAvanzaTomNook * deltaTime;
					rotaTomNook -= offsetGiroTomNook * deltaTime;
				}
				else {
					adelanteTomNook = true;
				}
			}
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tomNook.RenderModel();



		//=== Bolsa de dinero ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 2.0f + 4 * sin(glm::radians(movBolsaArribaAbajo)), -80.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, rotacionBolsaEje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		if (activarAnimacionBolsa == true) {
			rotacionBolsaEje += offsetGiroBolsa * deltaTime;
			movBolsaArribaAbajo += offsetBolsaArribaAbajo * deltaTime;
		}
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bolsaDinero.RenderModel();

		//=== Pez - Animacion Salto ===
		model = glm::mat4(1.0);
		float componenteYpez = -1.5f + 5 * compSeno;
		compSeno = sin(glm::radians(movPezY));
		model = glm::translate(model, glm::vec3(-140.0f + movPezX, componenteYpez, 100.0f + movPezZ));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//Lineas que hacebn que no se inicialice desde coordenadas muy lejanas
		if (movPezZ > 70.0f) {
			movPezZ = 0.0f;
		}

		//rotacion En circuito
		model = glm::rotate(model, rotaPez * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (adelantePez == true) {
			if (movPezZ < 25.0f)
			{
				//Subida de salto
				if (movPezZ > -22 and movPezZ < 15) {
					movPezY += offsetPezArribaAbajo * deltaTime;

				}
				movPezZ += offsetAvanzaPez * deltaTime;
			}
			else if (movPezZ >= 25.0 and movPezZ < 28.0f) {
				movPezZ += 0.1 * deltaTime;
				rotaPez += offsetGiroPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else if (movPezX < 30.0f) {
				movPezX += offsetAvanzaPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else if (movPezX >= 30.0 and movPezX < 33.0f) {
				movPezX += 0.1 * deltaTime;
				rotaPez += offsetGiroPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else {
				adelantePez = false;
				movPezY = 270;
			}

		}
		else {
			if (movPezZ > -25.0f)
			{
				movPezZ -= offsetAvanzaPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else if (movPezZ <= -25.0 and movPezZ > -28.0f) {
				movPezZ -= 0.1 * deltaTime;
				rotaPez += offsetGiroPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else if (movPezX > -20.0f) {
				movPezX -= offsetAvanzaPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else if (movPezX <= -20.0 and movPezX > -23.0f) {
				movPezX -= 0.1 * deltaTime;
				rotaPez += offsetGiroPez * deltaTime;
				compSeno = -1.0f;
				movPezY = 270;
			}
			else {
				adelantePez = true;
				compSeno = -1.0f;
				movPezY = 270;
			}
		}
		if (movPezZ > 359.0) {
			pezOffset = 0.0f;
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezRojoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PezRojo_M.RenderModel();

		//=== Leviathan ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, 0.0f + resultado, movLevZ));
		model = glm::scale(model, glm::vec3(200.0f, 200.0f, 200.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -35 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//Lineas que hacebn que no se inicialice desde coordenadas muy lejanas
		if (movLevZ > 40.0f) {
			movLevZ = 0.0f;
			rotaLev = 0.0f;
		}
		model = glm::rotate(model, rotaLev * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		//Segunda ejecución
		if (controlDireccionLev == true) {
			if (movLevZ >= -27.0f) {
				movLevZ -= levOffset * deltaTime;
				resultado = std::abs(0.02f * std::pow(movLevZ, 2) - 2) - 8;
				rotaLev += offsetGiroLev * deltaTime;
			}
			else {
				controlDireccionLev = false;
			}
		}
		else {
			if (movLevZ <= 27.0f) {
				movLevZ += levOffset * deltaTime;
				resultado = std::abs(0.02f * std::pow(movLevZ, 2) - 2) - 8;
				rotaLev -= offsetGiroLev * deltaTime;
			}
			else {
				controlDireccionLev = true;
			}
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LeviathanTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Leviathan_M.RenderModel();

		//=== Capsulas ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, 0.0f, 80.0));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CapsulasTexture.UseTexture();
		Capsulas_M.RenderModel();

		//=== Sombrilla ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.0f, 115.0));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		UmbrellaTexture.UseTexture();
		Umbrella_M.RenderModel();

		//=== Silla de playa ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -2.0f, 110.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BeachChair_M.RenderModel();
		
		//=== Red ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 113.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NetTexture.UseTexture();
		Net_M.RenderModel();


		//=== Primitiva: Agua ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-300.0f, -102.0f, 300.0));		//x:- (EscalaX/2)  y:- (EscalaY+2)  z: EscalaZ/2
		model = glm::scale(model, glm::vec3(600.0f, 100.0f, 600.0f));

		//blending: transparencia o traslucidez  menor al 100%
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguaTexture.UseTexture();
		meshList[10]->RenderMesh();
		glDisable(GL_BLEND);


		//=== Texto [Textura con movimiento] === 
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001;
		toffsetv += 0.0;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		//if (toffsetv > 1.0)   //movimiento vertical
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetu, toffsetv);

		model = glm::mat4(1.0);
		model = glm::translate(model, posicionLetreroCasaTomNook + glm::vec3(29.0f, 27.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glEnable(GL_BLEND);  //habilita las texturas del agave para blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		texto.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		glDisable(GL_BLEND);


		//// Calcular la posici�n del objeto en la espiral
		//float x = spiralRadius * cos(t);
		//float y = spiralRadius * sin(t);
		//float z = t * spiralHeight;


		//=== Dron ===
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXAnimacion + movAnimacion_x, posYAnimacion + movAnimacion_y, posZAnimacion + movAnimacion_z);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, giroAnimacion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroDosAnimacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, giroTresAnimacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dron.RenderModel();

		//Helice1 dron
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		heliceDron.RenderModel();

		//=== Faro ===
		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(30.0f, -3.0f, 50.0));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		faro.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}



void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}

	//Reproducir animacion
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	//Guardar Frame
	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			guardoFrame++;
			reinicioFrame = 0;
		}
	}

	//Escribir frames en archivo
	if (keys[GLFW_KEY_E])
	{
		if (guardoFrame < 1)
		{
			archivo1.Escribir();
			guardoFrame++;
			reinicioFrame = 0;
		}
	}

	//Control para habilotar lo anterior
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}

	//Movimiento de X positivo
	if (keys[GLFW_KEY_G])
	{
		if (ciclo < 1)
		{
			movAnimacion_x += 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de X negativo
	if (keys[GLFW_KEY_F])
	{
		if (ciclo < 1)
		{
			movAnimacion_x -= 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}




	//Movimiento de Y positivo
	if (keys[GLFW_KEY_T])
	{
		if (ciclo < 1)
		{
			movAnimacion_y += 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Y negativo
	if (keys[GLFW_KEY_V])
	{
		if (ciclo < 1)
		{
			movAnimacion_y -= 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}


	//Movimiento de Z positivo
	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			movAnimacion_z += 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Z negativo
	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			movAnimacion_z -= 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}



	//Movimiento de Giro positivo
	if (keys[GLFW_KEY_J])
	{
		if (ciclo < 1)
		{
			giroAnimacion += 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Giro negativo
	if (keys[GLFW_KEY_H])
	{
		if (ciclo < 1)
		{
			giroAnimacion -= 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}


	//Movimiento de Giro positivo segundo EJE
	if (keys[GLFW_KEY_U])
	{
		if (ciclo < 1)
		{
			giroDosAnimacion += 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Giro negativo segundo EJE
	if (keys[GLFW_KEY_Y])
	{
		if (ciclo < 1)
		{
			giroDosAnimacion -= 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}



	//Movimiento de Giro positivo tercer EJE
	if (keys[GLFW_KEY_N])
	{
		if (ciclo < 1)
		{
			giroTresAnimacion += 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Giro negativo tercer EJE
	if (keys[GLFW_KEY_B])
	{
		if (ciclo < 1)
		{
			giroTresAnimacion -= 10.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}



	//Boton de control de todos los movimientos
	if (keys[GLFW_KEY_1])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}



	//Animaciones básicas

	//Activar o desactivar Animacion Pengling
	if (keys[GLFW_KEY_7])
	{
		if (activarBooleano < 1)
		{
			activarBooleano++;
			habilitarBooleano = 0;
			activarAnimacionPengling = !activarAnimacionPengling;
		}
	}

	//Activar o desactivar Animacion Tom Nook
	if (keys[GLFW_KEY_8])
	{
		if (activarBooleano < 1)
		{
			activarBooleano++;
			habilitarBooleano = 0;
			activarAnimacionTomNook = !activarAnimacionTomNook;
		}
	}

	//Activar o desactivar Animacion Bolsa de dinero
	if (keys[GLFW_KEY_9])
	{
		if (activarBooleano < 1)
		{
			activarBooleano++;
			habilitarBooleano = 0;
			activarAnimacionBolsa = !activarAnimacionBolsa;
		}
	}

	//Control para habilitar lo anterior
	if (keys[GLFW_KEY_2])
	{
		if (habilitarBooleano < 1)
		{
			activarBooleano = 0;
		}
	}




}

//Prueba de GIT
//Prueba commit ignore Github