/*
Semestre 2023-2
Proyecto: Diorama de la vida cotidiana -  Subnautica
Integrantes
* Castillo Montes Pamela			Gpo. 10
* Hernández Jaimes Rogelio Yael		Gpo. 10
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

//Encabezados Iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//Encabezados Animación Keyframes
#include "Archivos.h"
#include "Frame.h"


//numeros random
#include<cstdlib>
using namespace std;


// === Constantes Ángulos === 
const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;

// === Variables Animación Simple === 
float movCoche;
float movCocheY;
float movCocheZ;
float rotacionRampa;
float rotacionCurva;


float movOffset;
float movOffsetY;
float movOffsetZ;
float movOffsetSubeRampa;
float movOffsetRotacionRampa;
float movOffsetRotacionCurva;


float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetu = 0.0f;
float toffsetv = 0.0f;

float movHeli = 0.0f;
float movHeliX = 0.0f;
float rotaHeli = 0.0f;

float heliOffset = 0.0f;  //grados de subir y bajar
float subeBajaHeli = 0.0f;
float offsetAvanzaHeli = 0.0f;
float offsetGiroHeli = 0.0f;

//movimiento de dado
float movDadoArriba = 0.0f;
float offsetDadoArriba = 0.0f;
int presionaTecla = 0;
int presionateclaPrevio = 0;

//Movimiento de pengling
float offsetAvanzaPengling;
float offsetGiroPengling;
float penglingOffset;
float movPenglingZ = 0.0f;
float movPenglingX = 0.0f;
float rotaPengling = 0.0f;

//Movimiento de la Bolsa de Dinero
float rotacionBolsaEje;
float offsetGiroBolsa;
float movBolsaArribaAbajo;
float offsetBolsaArribaAbajo;


// === Variables Animación Keyframes ===
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

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
Texture AgaveTexture;
Texture FlechaTexture;
Texture texto;
Texture dadoTexture;

Texture mesaTexture;
Texture aguaTexture;
Texture arenaTexture;

// === Variables de Modelos ===
//Coche
Model cochePropio;
Model ruedaSupDer;
Model ruedaSupIzq;
Model ruedaInfDer;
Model ruedaInfIzq;

//Pista
Model pista;

//Práctica
Model Kitt_M;
Model Llanta_M;
Model Camino_M;

//Helicoptero
Model Blackhawk_M;
Model helice;
Model Dado_M;
Model papalote;

//Iluminación
Model faro;
Model antorcha;

//Pingu
Model Pingu;

//Bolsa de dinero
Model bolsaDinero;

//Leviathan
Model Leviathan_M;

Skybox skybox;

// === Materiales ===
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//  === Luz direccional === 
DirectionalLight mainLight;

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



/*Cálculo del promedio de las normales para sombreado de Phong
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

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

//función para crear pirámide cuadrangular unitaria
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
		 // Vértice de la punta
		  0.0f, 1.0f,  0.0f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f  // 4
	};

	//mesh[]
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramide_vertices, piramide_indices, 40, 18);
	meshList.push_back(piramide);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i, coordenada = 3;
	//número de vértices ocupados
	int verticesBase = (res) * 3;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	//apuntadores para guardar todos los vértices e índices generados
	GLfloat* vertices = (GLfloat*)calloc(sizeof(GLfloat*), (verticesBase)+6);
	unsigned int* indices = (unsigned int*)calloc(sizeof(unsigned int*), res + 1);
	//caso inicial para crear el cono
	vertices[0] = 0.0f;
	vertices[1] = 0.5f;
	vertices[2] = 0.0f;
	//ciclo for para crear los vértices de la circunferencia del cono
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
	//se agregan las coordenadas del último vértice para completar la circunferencia
	vertices[coordenada + 1] = R * cos(0) * dt;
	vertices[coordenada + 2] = -0.5f;
	vertices[coordenada + 3] = R * sin(0) * dt;;

	coordenada += 3;

	//se agregan los índices
	for (i = 0; i < res + 2; i++) {
		indices[i] = i;
	}

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMesh(vertices, indices, coordenada, res + 2);
	meshList.push_back(cono);
}


//Función crea primitiva: mesa
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

//Función crea primitiva: agua
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

//Función crea primitiva: arena
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


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}





///////////////////////////////KEYFRAMES/////////////////////  

bool animacion = false;


//NEW// Keyframes
float posXAnimacion = 43.0, posYAnimacion = 20.0, posZAnimacion = 0.0;
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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	// === Texturas ===
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pisoAzul.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();

	texto = Texture("Textures/texto.tga");
	texto.LoadTextureA();
	dadoTexture = Texture("Textures/dado8.jpg");
	//dadoTexture.LoadTextureA();
	dadoTexture.LoadTexture();

	mesaTexture = Texture("Textures/Wood.tga");
	mesaTexture.LoadTexture();
	aguaTexture = Texture("Textures/Agua.tga");
	aguaTexture.LoadTextureA();
	arenaTexture = Texture("Textures/Arena.tga");
	arenaTexture.LoadTexture();

	// === Carga de modelos ===
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/helicopteroSinHelice.obj");
	helice = Model();
	helice.LoadModel("Models/helice.obj");

	faro = Model();
	faro.LoadModel("Models/Pruebas/Street Lamp.obj");
	antorcha = Model();
	antorcha.LoadModel("Models/Pruebas/Torch.obj");
	
	//Coche propio  NISSAN
	cochePropio = Model();
	//cochePropio.LoadModel("Models/Nisssa_Figaro_1991_OBJ.obj");
	//cochePropio.LoadModel("Models/nissan.obj");

	//ruedas indpendientes
	ruedaSupDer = Model();
	ruedaSupDer.LoadModel("Models/ruedaSupDer.3ds");

	ruedaSupIzq = Model();
	ruedaSupIzq.LoadModel("Models/ruedaSupIzq.3ds");

	ruedaInfDer = Model();
	ruedaInfDer.LoadModel("Models/ruedaInfDer.3ds");

	ruedaInfIzq = Model();
	ruedaInfIzq.LoadModel("Models/ruedaInfIzq.3ds");

	//Pista
	pista = Model();
	pista.LoadModel("Models/pista2rellena.fbx");

	//Papalote
	papalote = Model();
	papalote.LoadModel("Models/kite sf.obj");

	//Pingu
	Pingu = Model();	
	Pingu.LoadModel("Models/Pingu.obj");

	//Bolsa de dinero
	bolsaDinero = Model();
	bolsaDinero.LoadModel("Models/monedero.obj");

	// === Skybox ===
	std::vector<std::string> skyboxFaces;
	//Day
	skyboxFaces.push_back("Textures/Skybox/Day_Right.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Left.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Top.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Front.tga");
	skyboxFaces.push_back("Textures/Skybox/Day_Back.tga");
	/*
	//Night
	skyboxFaces.push_back("Textures/Skybox/Night_Right.tga");
	skyboxFaces.push_back("Textures/Skybox/Night_Left.tga");
	skyboxFaces.push_back("Textures/Skybox/Night_Bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/Night_Top.tga");
	skyboxFaces.push_back("Textures/Skybox/Night_Front.tga");
	skyboxFaces.push_back("Textures/Skybox/Night_Back.tga");
	*/

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	// === Luz Direccional === 
	//Solo 1 y siempre debe de existir

	//1° Luz [Direccional]: Sol
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.4f,
		0.0f, 0.0f, -1.0f);


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

	//1° Luz [Reflector]: Luz frontal del coche
	spotLightsInverso[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//2° Luz [Reflector]: Luz trasera del coche
	spotLightsInverso[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//3° Luz [Reflector]: Luz del helicoptero
	spotLightsInverso[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);

	//Variables Uniform
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
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


	//Inicialización variables animación
	movCoche = 0.0f;
	movCocheY = 0.0f;
	movCocheZ = 0.0f;
	rotacionRampa = 0.0f;
	rotacionCurva = 0.0f;

	movOffset = 0.7f;
	movOffsetY = 0.32f;
	movOffsetZ = 0.3f;
	movOffsetSubeRampa = 0.5f;
	movOffsetRotacionRampa = 1.1f;
	movOffsetRotacionCurva = 1.0f;

	bool segundaCurvaParteDos = false;
	bool rectaFinal = false;

	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	offsetAvanzaHeli = 0.2f;
	offsetGiroHeli = 3.0f;
	heliOffset = 5.0f;  //es el senoidal
	float rotaHelice = 0.0f;
	bool adelanteCoche = true;
	int i = 0;
	bool adelanteHeli = true;

	//Movimiento pengling
	offsetAvanzaPengling = 0.4f;
	offsetGiroPengling = 3.0f;
	penglingOffset = 5.0f;  //es el senoidal
	bool adelantePengling = true;

	//Movimiento de bolsa de dinero
	offsetGiroBolsa = 4;
	rotacionBolsaEje = 0.0;
	offsetBolsaArribaAbajo = 4.0f;
	movBolsaArribaAbajo = 0.0f;
	
	//dado
	movDadoArriba = 20.5f;
	offsetDadoArriba = 0.4f;
	bool nuevoTiro = false;
	bool resultadoDado = false;
	int random = 1;

	//parametros de Arquimedes
	// Variables para la animación en espiral de Arquímedes
	float t = 0.0f;                     // Parámetro t de la espiral
	float spiralRadius = 2.0f;          // Radio de la espiral
	float spiralHeight = 0.1f;         // Altura de la espiral
	float spiralSpeed = 0.15f;           // Velocidad de la animación

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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		
		uniformTextureOffset = shaderList[0].getOffsetLocation();


		//Información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		//=== luz ligada cámara [flash] ===
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//=== Modelos ===
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxHelice(1.0);

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		//=== Pista ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(18.0f, 18.0f, 18.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::rotate(model, -2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		brickTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pista.RenderModel();

		//=== Antorcha ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 2.0f, 165.0));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		antorcha.RenderModel();


		// ========= PROYECTO =========

		//=== Primitiva: Mesa ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 2.0f, 165.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesaTexture.UseTexture();
		meshList[9]->RenderMesh();

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
			

		//=== Primitiva: Arena ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-300.0f, -102.0f, 300.0));		//x:- (EscalaX/2)  y:- (EscalaY+2)  z: EscalaZ/2
		model = glm::scale(model, glm::vec3(600.0f, 100.0f, 600.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arenaTexture.UseTexture();
		meshList[11]->RenderMesh();

	
		//=== Pingu: Modelo de pinguino ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f + movPenglingX, -2.0f , 200.0 + movPenglingZ));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//rotacion En circuito
		model = glm::rotate(model, rotaPengling * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Movimiento del pinguino
		if (adelantePengling == true) {
			//rotaHelice += 6 * deltaTime;
			if (movPenglingZ < 15.0f)
			{
				movPenglingZ += offsetAvanzaPengling * deltaTime;
			}
			else if (movPenglingZ >= 15.0 and movPenglingZ < 18.0f) {
				movPenglingZ += 0.1 * deltaTime;
				rotaPengling += offsetGiroPengling * deltaTime;
			}
			else if (movPenglingX < 20.0f) {
				movPenglingX += offsetAvanzaPengling * deltaTime;
			}
			else if (movPenglingX >= 20.0 and movPenglingX < 23.0f) {
				movPenglingX += 0.1 * deltaTime;
				rotaPengling += offsetGiroPengling * deltaTime;
			}
			else {
				adelantePengling = false;
			}

		}
		else {
			//rotaHelice += 6 * deltaTime;
			if (movPenglingZ > -15.0f)
			{
				movPenglingZ -= offsetAvanzaPengling * deltaTime;
			}
			else if (movPenglingZ <= -15.0 and movPenglingZ > -18.0f) {
				movPenglingZ -= 0.1 * deltaTime;
				rotaPengling += offsetGiroPengling * deltaTime;
			}
			else if (movPenglingX > -10.0f) {
				movPenglingX -= offsetAvanzaPengling * deltaTime;
			}
			else if (movPenglingX <= -10.0 and movPenglingX > -13.0f) {
				movPenglingX -= 0.1 * deltaTime;
				rotaPengling += offsetGiroPengling * deltaTime;
			}
			else {
				adelantePengling = true;
				//rotaHelice = 0.0;
			}
		}
		//subeBajaPengling += penglingOffset * deltaTime;
		if (movPenglingZ > 359.0) {
			penglingOffset = 0.0f;
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pingu.RenderModel();



		//=== Bolsa de dinero ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 2.0f + 4*sin(glm::radians(movBolsaArribaAbajo)), 180.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, rotacionBolsaEje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		rotacionBolsaEje += offsetGiroBolsa * deltaTime;

		movBolsaArribaAbajo += offsetBolsaArribaAbajo * deltaTime;
		/*if (movBolsaArribaAbajo > 359.0) {
			offsetBolsaArribaAbajo = 0.0f;
		}*/

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bolsaDinero.RenderModel();


		/*
		//=== Piso [Textura con movimiento] === 
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
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		////blending: transparencia o traslucidez  menor al 100%
		glEnable(GL_BLEND);  //habilita las texturas del agave para blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);
		*/

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
		model = glm::translate(model, glm::vec3(-20.0f, 0.12f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
		glEnable(GL_BLEND);  //habilita las texturas del agave para blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		texto.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		glDisable(GL_BLEND);
		

		//// Calcular la posición del objeto en la espiral
		//float x = spiralRadius * cos(t);
		//float y = spiralRadius * sin(t);
		//float z = t * spiralHeight;
		

		//=== Papalote ===
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXAnimacion + movAnimacion_x, posYAnimacion + movAnimacion_y, posZAnimacion + movAnimacion_z);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, giroAnimacion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroDosAnimacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, giroTresAnimacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		papalote.RenderModel();

		//// Actualizar el parámetro t para avanzar en la espiral
  //      t += deltaTime * spiralSpeed;
		

		//=== Cubo ===
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(4.6f, 10.0f, -53.0f));
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));    //rotar para ver tapa
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));    //rotar para ver tapa
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[7]->RenderMesh();


		//=== Piramide ===
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::translate(model, glm::vec3(4.6f, 20.0f, -53.0f));
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));    //rotar para ver tapa
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));    //rotar para ver tapa
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[8]->RenderMesh();


		//=== Faro ===
		model = glm::mat4(1.0);
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(30.0f, -3.0f, 50.0));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		faro.RenderModel();


		//=== Helicoptero ===
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movHeliX, 63.0f+sin(glm::radians(subeBajaHeli)), -1.0+movHeli));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f , -1.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f ));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		modelauxHelice = model;
		modelauxHelice = glm::rotate(modelauxHelice, rotaHelice * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		//rotacion En circuito
		model = glm::rotate(model, rotaHeli * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));


		//Movimiento del helicoptero
		if (adelanteHeli == true) {
			rotaHelice +=  6 * deltaTime;
			if (movHeli < 15.0f)
			{
				movHeli += offsetAvanzaHeli * deltaTime;
			}
			else if (movHeli >= 15.0 and movHeli < 18.0f) {
				movHeli += 0.1 * deltaTime;
				rotaHeli += offsetGiroHeli * deltaTime;
			}
			else if (movHeliX < 20.0f) {
				movHeliX += offsetAvanzaHeli * deltaTime;
			}
			else if (movHeliX >= 20.0 and movHeliX < 23.0f) {
				movHeliX += 0.1 * deltaTime;
				rotaHeli += offsetGiroHeli * deltaTime;
			}
			else {
				adelanteHeli = false;
			}

		}
		else {
			rotaHelice += 6 * deltaTime;
			if (movHeli > -15.0f)
			{
				movHeli -= offsetAvanzaHeli * deltaTime;
			}
			else if (movHeli <= -15.0 and movHeli > -18.0f) {
				movHeli -= 0.1 * deltaTime;
				rotaHeli += offsetGiroHeli * deltaTime;
			}
			else if (movHeliX > -10.0f) {
				movHeliX -= offsetAvanzaHeli * deltaTime;
			}
			else if (movHeliX <= -10.0 and movHeliX > -13.0f) {
				movHeliX -= 0.1 * deltaTime;
				rotaHeli += offsetGiroHeli * deltaTime;
			}
			else {
				adelanteHeli = true;
				rotaHelice = 0.0;
			}
		}
		subeBajaHeli += heliOffset * deltaTime;
		if (movHeli > 359.0) {
			heliOffset = 0.0f;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelauxHelice));
		helice.RenderModel();

		spotLightsInverso[2].SetFlash(glm::vec3(0.0f + movHeliX, 63.0f + sin(glm::radians(subeBajaHeli)), -1.0 + movHeli), glm::vec3(0.0f, -1.0f, 0.0f));
		shaderList[0].SetSpotLightsManual(spotLightsInverso, 2);
		

		//=== Coche ===
		float posicionInicialCarro = 200.0f;

		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::mat4(1.0);
		//movimiento de la carrocería
		//model = glm::translate(model, glm::vec3(posicionInicialCarro + movCoche, -269.7f+movCocheY, 0.0f));
		model = glm::translate(model, glm::vec3(posicionInicialCarro + movCoche, 3.7f + movCocheY, 0.0f + movCocheZ));
		//model = glm::translate(model, glm::vec3(posicionInicialCarro, -1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//Rotacion rampa
		model = glm::rotate(model, -(rotacionRampa) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		//Rotacion cuvas
		model = glm::rotate(model, -(rotacionCurva)*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//Movimiento del Coche
			//primera recta
		if (adelanteCoche == true) {
			if (movCoche > -156.0f)
			{
				movCoche -= movOffset * deltaTime;
				//cuando comienza a avanzar
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//rotacion subida
			else if (movCoche > -173.0f and movCoche <= -156.0f)
			{
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
				movCoche -= movOffsetSubeRampa * deltaTime;
				movCocheY += movOffsetY * deltaTime;
				rotacionRampa += movOffsetRotacionRampa * deltaTime;
			}
			//plano subida empinado
			else if (movCoche > -190.0f and movCoche <= -173.0f)
			{
				movCoche -= movOffsetSubeRampa * deltaTime;
				movCocheY += movOffsetY * deltaTime;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, 1.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//rotacion para el plano superior
			else if (movCoche > -207.0f and movCoche <=-190.0f)
			{
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
				movCoche -= movOffsetSubeRampa * deltaTime;
				rotacionRampa -= movOffsetRotacionRampa * deltaTime;
			}
			//plano superior
			else if (movCoche > -256.0f and movCoche <= -204.0f)
			{
				movCoche -= movOffsetSubeRampa * deltaTime;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//BAJADA
			//rotacion bajada
			else if (movCoche > -273.0f and movCoche <= -256.0f)
			{
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
				movCoche -= movOffsetSubeRampa * deltaTime;
				movCocheY -= movOffsetY * deltaTime;
				rotacionRampa -= movOffsetRotacionRampa * deltaTime;
			}
			//plano bajada empinado
			else if (movCoche > -295.0f and movCoche <= -273.0f)
			{
				movCoche -= movOffsetSubeRampa * deltaTime;
				movCocheY -= movOffsetY * deltaTime;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, -1.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//rotacion para el plano inferior
			else if (movCoche > -312.0f and movCoche <= -295.0f)
			{
				movCoche -= movOffsetSubeRampa * deltaTime;
				//movCocheY -= movOffsetY * deltaTime;
				rotacionRampa += movOffsetRotacionRampa * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//plano inferior
			else if (movCoche > -488.0f and movCoche <= -312.0f)
			{
				movCoche -= movOffset * deltaTime;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}


			//CURVA
			//Primera parte
			else if (movCoche > -530.0f and movCoche <= -488.0f)
			{
				movCoche -= movOffsetSubeRampa * deltaTime;
				movCocheZ += 0.32f * deltaTime;
				rotacionCurva -= movOffsetRotacionCurva * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);

			}
			else if(movCocheZ < 37.0f){
				movCocheZ += movOffsetZ * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}

			else {
				adelanteCoche = false;
				movCocheY = 0;
			}
			rotllanta += rotllantaOffset * deltaTime;
		}
		else {
			//CURVA
			//segunda parte
			if (movCoche < -488.0f)
			{
				movCoche += movOffsetSubeRampa * deltaTime;
				movCocheZ += movOffsetZ * deltaTime;
				rotacionCurva -= 1.13f * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
				movCocheY = -2.5f;

			}
			//Recta de regreso
			else if (movCoche < 43.0f and rectaFinal == false) {
				movCoche += movOffset * deltaTime;
				movCocheY += 0.0045 * deltaTime;
				segundaCurvaParteDos = false;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(1.0f, 0.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//SEGUNDA CURVA (de regreso)
			//primera parte
			else if (movCoche >= 43.0f and movCoche <= 63.0f and segundaCurvaParteDos == false and rectaFinal == false) {
				movCoche += 0.25 * deltaTime;
				movCocheZ -= 0.25 * deltaTime;
				rotacionCurva -= 1.21f * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//recta curva 
			else if (movCocheZ > 23.0f) {
				movCocheZ -= 0.25 * deltaTime;
				segundaCurvaParteDos = true;
				rectaFinal = true;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//segunda parte
			else if (movCoche >= 48.0f and segundaCurvaParteDos == true and rectaFinal == true) {
				movCoche -= 0.18 * deltaTime;
				movCocheZ -= 0.27 * deltaTime;
				rotacionCurva -= 0.96f * deltaTime;
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}
			//Recta final
			else if (movCoche < 48.0f and movCoche > 0 and rectaFinal == true) {
				movCoche -= movOffset * deltaTime;
				spotLightsInverso[0].SetFlash(glm::vec3(posicionInicialCarro + movCoche, 4.5f + movCocheY, -1.0f + movCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
				shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
				shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);
			}


			else {
				adelanteCoche = true;
				movCoche = 0;
				movCocheY = 0;
				movCocheZ = 0;
				rotacionCurva = 0;
				rectaFinal = false;
			}

		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cochePropio.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posicionInicialCarro + movCoche, 6.5f + movCocheY, 0.0f + movCocheZ));
		//Rotacion rampa
		model = glm::rotate(model, -(rotacionRampa)*toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//Rotacion cuvas
		model = glm::rotate(model, -(rotacionCurva)*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;

		//rueda superior derecha
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.7f, -1.88f, -3.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		//giro
		model = glm::rotate(model, -5 * movCoche * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaSupDer.RenderModel();

		//rueda superior izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.7f, -1.88f, 3.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//giro
		model = glm::rotate(model, 5 * movCoche * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaSupIzq.RenderModel();

		//rueda inferior derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.7f, -1.88f, -3.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		//giro
		model = glm::rotate(model, -5 * movCoche * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaInfDer.RenderModel();

		//rueda inferior izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.7f, -1.88f, 3.2f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//giro
		model = glm::rotate(model, 5 * movCoche * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ruedaInfIzq.RenderModel();



		// === Agave ===
		//¿qué sucede si lo renderizan antes del coche y de la pista?
		////se deben renderizar al final para ver las texturas con traslucidez
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////blending: transparencia o traslucidez  menor al 100%
		//glEnable(GL_BLEND);  //habilita las texturas del agave para blending
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//AgaveTexture.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		

		// === textura con movimiento ===
		//// 
		//// Va al final para que no se muevan todas las texturas
		//// Se tendrian que inicializar en 0 los off para agregar movimiento en otras texturas
		//// 
		////Importantes porque la variable uniform no podemos modificarla directamente
		//toffsetu += 0.001;
		//toffsetv += 0.0;
		////para que no se desborde la variable
		//if (toffsetu > 1.0)
		//	toffsetu = 0.0;
		////if (toffsetv > 1.0)   //movimiento vertical
		////	toffsetv = 0;
		////printf("\ntfosset %f \n", toffsetu);
		////pasar a la variable uniform el valor actualizado
		//toffset = glm::vec2(toffsetu, toffsetv);

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.2f, -6.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//
		//FlechaTexture.UseTexture();
		////Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();
		//glDisable(GL_BLEND);
		
		



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
	if (keys[GLFW_KEY_R])
	{
		if (ciclo < 1)
		{
			movAnimacion_z += 1.0f;
			ciclo++;
			ciclo2 = 0;
		}

	}
	//Movimiento de Z negativo
	if (keys[GLFW_KEY_C])
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

}

