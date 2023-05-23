#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glew.h>
#include "CommonValues.h"
#include <regex>
#include "Frame.h"


class Archivo
{
public:
	Archivo();

	void Escribir();
	void LeerKeyFrames(std::string cadenas);
	void LeerKeyFramesDeArchivo();
	bool archivoEstaVacio();

	~Archivo();

private:
	int archivoID;
	std::string contenido;

};