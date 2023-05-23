/*
Semestre 2023-2
Práctica : Iluminación
Cambios en el shader: 
en lugar de enviar solo información de color e información de la textura en el shader de fragmentos,
enviaremos también información de iluminación almacenada en finalcolor.
Modelo de Iluminación de Lambert/Sombreado de Phong 3 componentes: Ambiental, Difusa, Especular
Archivos nuevos a utilizar:
CommonValues.h: Contiene banderas de número máximo de fuentes de iluminación puntual y spotlight
DirectionalLight: Luz direccional
PointLight Luces Puntuales 
Spotlight Luces Spotlight
Material Información de intensidad especular y brillo, se puede modificar para agregar 
componentes de color difuso, en el caso del laboratorio lo manejo por el color de la variable uniform
*/

//Práctica 7. Marco Antonio Sánchez Pérez
//Para mover el coche se usan las teclas: 'U' y 'Y' para adelante y atrás.
//Para mover el coche se usan las teclas: 'T' y 'G' para movimiento en eje Z (hacia los lados)

//Para mover el helicptero se usan las teclas: 'K' y 'M' para arriba y abajo
//Para mover el helicptero se usan las teclas: 'J' y 'L' para atrás y adelante

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


//para el tiempo
#include <chrono>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;



//Carro practica: Carro, y ruedas
Model cochePropio;
Model ruedaSupDer;
Model ruedaSupIzq;
Model ruedaInfDer;
Model ruedaInfIzq;
//Model puertaCarro;

//lampara
Model lampara1;
Model lampara2;



//pista
Model pista;
Texture pistaTexture;


Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightsInverso[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


/*Cálculo del promedio de las normales para sombreado de Phong
en algunas ocasiones nos ayuda para no tener que declarar las normales manualmente dentro del VAO
*/
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
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




void CreateObjects() // el plano de piso, una planta
{
	
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		//x,y,z                 textura          normal en -y
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
	
	
	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	//se usa para tener normales automaticas
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);




	//piso de la pista: Mesh[2]
	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(pistaVertices, pistaIndices, 32, 6);
	meshList.push_back(obj5);

}

//void CrearCubo()
//{
//	unsigned int cubo_indices[] = {
//		// front
//		0, 1, 2,
//		2, 3, 0,
//		// right
//		4, 5, 6,
//		6, 7, 4,
//		// back
//		8, 9, 10,
//		10, 11, 8,
//
//		// left
//		12, 13, 14,
//		14, 15, 12,
//		// bottom
//		16, 17, 18,
//		18, 19, 16,
//		// top
//		20, 21, 22,
//		22, 23, 20,
//	};
//
//
//	GLfloat cubo_vertices[] = {
//		// front
//		//x		y		z		S		T			NX		NY		NZ
//		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
//		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
//		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
//		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
//		// right
//		//x		y		z		S		T
//		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
//		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
//		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
//		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
//		// back
//		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
//		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
//		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
//		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,
//
//		// left
//		//x		y		z		S		T
//		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
//		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
//		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
//		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,
//
//		// bottom
//		//x		y		z		S		T
//		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
//		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
//		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
//		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,
//
//		//UP
//		 //x		y		z		S		T
//		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
//		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
//		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
//		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,
//
//	};
//
//	Mesh *cubo = new Mesh();
//	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
//	meshList.push_back(cubo);
//
//}
//

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

	//
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}




void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();

	//cambio de velocidad ultim valor
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	//dadoTexture = Texture("Textures/dado.tga");
	//dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();




	dadoTexture = Texture("Textures/dado8.jpg");
	//dadoTexture.LoadTextureA();
	dadoTexture.LoadTexture();




	pistaTexture = Texture("Textures/pista.tga");
	pistaTexture.LoadTextureA();




	//Coche propio  NISSAN
	cochePropio = Model();
	cochePropio.LoadModel("Models/Nisssa_Figaro_1991_OBJ.obj");

	//ruedas indpendientes
	ruedaSupDer = Model();
	ruedaSupDer.LoadModel("Models/ruedaSupDer.obj");

	ruedaSupIzq = Model();
	ruedaSupIzq.LoadModel("Models/ruedaSupIzq.obj");

	ruedaInfDer = Model();
	ruedaInfDer.LoadModel("Models/ruedaInfDer.obj");

	ruedaInfIzq = Model();
	ruedaInfIzq.LoadModel("Models/ruedaInfIzq.obj");

	////Lampara1
	//lampara1 = Model();
	//lampara1.LoadModel("Models/CONTE LG1.obj");

	////Lampara2
	//lampara2 = Model();
	//lampara2.LoadModel("Models/lampara2.obj");



	//pista01
	pista = Model();
	pista.LoadModel("Models/pista.fbx");



	//
	//
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");


	//Camino_M = Model();
	//Camino_M.LoadModel("Models/railroad track.obj");

	//Dado_M = Model();
	//Dado_M.LoadModel("Models/dadoanimales.obj");



	/*Dado_M = Model();
	Dado_M.LoadModel("Models/dado.obj");*/



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//luz direccional, sólo 1 y siempre debe de existir (LUZ DEL SOL)
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,  //parametros de luz ambiental y componente difusa
		0.0f, 0.0f, -1.0f);



	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual: Lampara1
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		//2.0f, 1.5f, 1.5f,
		170.0f, 3.5f, 100.0f,
		0.5f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración de segunda luz puntual: Lampara2
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		0.8f, 1.0f,
		//2.0f, 1.5f, 1.5f,
		166.0f, 5.5f, 136.0f,
		0.9f, 0.4f, 0.2f);
	pointLightCount++;



	unsigned int spotLightCount = 0;
	////linterna original
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;


	////luz de helicóptero
	//spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f,
	//	0.5f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.03f, 0.0001f,
	//	15.0f);
	//spotLightCount++;


	//luz coche
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);*/

	//luz frontal del coche
	spotLightsInverso[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//Luz trasera del coche
	spotLightsInverso[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	spotLightCount++;

	//Luz del helicoptero
	spotLightsInverso[2] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.03f, 0.0001f,
		15.0f);
	



	//variables para detectar movimiento en luz de coche adelante y atras
	float componenteX = 0;
	float componenteXprevio = 0;

	//variables para detectar movimiento en luz de coche en Z (a los lados)
	float componenteZ = 0;
	float componenteZprevio = 0;

	//variables para detectar movimiento en luz de helicoptero
	float componenteYhelicoptero = 0;
	float componenteYprevioHelicoptero = 0;

	//variables para detectar movimiento en luz de helicoptero
	float componenteXhelicoptero = 0;
	float componenteXprevioHelicoptero = 0;
		

	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	


	float conteoTiempo = 0.0f;
	int encendido = 0;


	
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

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformColor = shaderList[0].getColorLocation();
		//color blanco inicializado para todos los objetos a menos que se asigne nuevo color
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



		////luz ligada a la cámara de tipo flash 
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);


		//Material del piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();


		//pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 10.0f, 110.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //rotacion para pista 2
		model = glm::rotate(model, 270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pistaTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();
		pista.RenderModel();


		//Dado de Opengl de 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//Se aplica una rotación para dejar la punta hasta arriba
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[3]->RenderMesh();



		//logica de movimiento hacia arriba y abajo (helicoptero)
		float movimientoLuzHelicptero = 0;
		movimientoLuzHelicptero = mainWindow.getmuevey();
		//logica de movimiento hacia adelante y atras (helicoptero)
		float movimientoLuzHelicpteroX = 0;
		movimientoLuzHelicpteroX = mainWindow.getmuevexHelicoptero();
		//logica de movimiento hacia adelante y atras (carro)
		float movimientoLuzCoche = 0;
		movimientoLuzCoche = mainWindow.getmuevex();
		//logica de movimiento hacia los lados (carro)
		float movimientoLuzCocheZ = 0;
		movimientoLuzCocheZ = mainWindow.getmuevezCarro();

		componenteX = 200.0f + movimientoLuzCoche;
		componenteZ = -1.5f + movimientoLuzCocheZ;
		componenteYhelicoptero = 6.0f + movimientoLuzHelicptero;
		componenteXhelicoptero = -20.0f + movimientoLuzHelicpteroX;


		//Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(componenteXhelicoptero, componenteYhelicoptero, -1.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//agregar material al helicóptero
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		//luz de helicoptero
		//spotLights[0].SetFlash(glm::vec3(-20.0f + movimientoLuzHelicpteroX, 4.0f + movimientoLuzHelicptero, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		spotLightsInverso[2].SetFlash(glm::vec3(-20.0f + movimientoLuzHelicpteroX, 4.0f + movimientoLuzHelicptero, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));



		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm::mat4(1.0);

		//movimiento de la carrocería
		model = glm::translate(model, glm::vec3(componenteX, -1.4f, componenteZ));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cochePropio.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(componenteX, 4.5f, componenteZ));
		modelaux = model;
		
		
		//rueda superior derecha
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.1f, 1.0f, 24.5));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ruedaSupDer.RenderModel();

		//rueda superior izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, 1.4f, -21.3));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ruedaSupIzq.RenderModel();

		//rueda inferior derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.1f, 0.8f, 37.5));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ruedaInfDer.RenderModel();

		//rueda inferior izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.1f, 0.8f, -34.5));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ruedaInfIzq.RenderModel();

		////puerta
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 11.5));
		//model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, glm::radians(mainWindow.getGiroPuerta()), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//puertaCarro.RenderModel();


		////Lamparas
		////lampara 1
		//color = glm::vec3(0.439f, 0.439f, 0.376f);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(170.0f, -1.5f, 100.0f));
		//model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//lampara1.RenderModel();

		////lampara 2
		//color = glm::vec3(0.239f, 0.239f, 0.176f);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(170.0f, -1.5f, 140.0f));
		//model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//lampara2.RenderModel();
		//
		//












		//Lógica que detecta movimiento en los vehiculos para encender las luces
		if ((componenteX < componenteXprevio)  ) {
			//se mueve el coche hacia adelante
			//spotLights[1].SetFlash(glm::vec3(194.0f + movimientoLuzCoche, 4.5f, -1.0f + movimientoLuzCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
			spotLightsInverso[0].SetFlash(glm::vec3(194.0f + movimientoLuzCoche, 4.5f, -1.0f + movimientoLuzCocheZ), glm::vec3(-1.0f, 0.0f, 0.0f));
			shaderList[0].SetSpotLightsManual(spotLightsInverso, 0);
		}
		else if ((componenteX > componenteXprevio)) {
			//se mueve el coche hacia atras
			//spotLights[1].SetFlash(glm::vec3(203.0f + movimientoLuzCoche, 4.5f, -1.0f + movimientoLuzCocheZ), glm::vec3(1.0f, 0.0f, 0.0f));
			spotLightsInverso[1].SetFlash(glm::vec3(203.0f + movimientoLuzCoche, 4.5f, -1.0f + movimientoLuzCocheZ), glm::vec3(1.0f, 0.0f, 0.0f));
			shaderList[0].SetSpotLightsManual(spotLightsInverso, 1);
		}
		else {
			shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 0);
			shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 1);


			//estructura de control de encendido y apagado de luz de helicoptero por tiempo
			if (conteoTiempo >= 60.0f) {
				if (encendido == 1) {
					//esta encendido, por lo que apaga
					shaderList[0].SetSpotLightsApagarUna(spotLightsInverso, 2);
					encendido = 0;
				}
				else {
					//esta apagado por lo que enciende
					//encendido = 0
					shaderList[0].SetSpotLightsManual(spotLightsInverso, 2);
					encendido = 1;

				}
				conteoTiempo = 0.0f;

			}
			conteoTiempo += 0.1f;
		}




		//else if ((componenteYhelicoptero > componenteYprevioHelicoptero)  ) {
		//	//solo se mueve el helicoptero en Y
		//	shaderList[0].SetSpotLightsManual(spotLights, 0);
		//}
		//else if ((componenteYhelicoptero < componenteYprevioHelicoptero)) {
		//	//solo se mueve el helicoptero en Y
		//	shaderList[0].SetSpotLightsManual(spotLights, 0);
		//}
		//else if ((componenteXhelicoptero > componenteXprevioHelicoptero)) {
		//	//solo se mueve el helicoptero en X
		//	shaderList[0].SetSpotLightsManual(spotLights, 0);
		//}
		//else if ((componenteXhelicoptero < componenteXprevioHelicoptero)) {
		//	//solo se mueve el helicoptero en X
		//	shaderList[0].SetSpotLightsManual(spotLights, 0);
		//}
		//else {
		//	//no se mueve nada
		//	shaderList[0].SetSpotLights(spotLights, spotLightCount-2);
		//	
		//}

		componenteXprevio = componenteX;
		componenteYprevioHelicoptero = componenteYhelicoptero;
		componenteXprevioHelicoptero = componenteXhelicoptero;


		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
