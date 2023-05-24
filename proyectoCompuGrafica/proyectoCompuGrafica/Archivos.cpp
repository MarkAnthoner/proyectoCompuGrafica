#include "Archivos.h"

Archivo::Archivo()
{
	archivoID = 0;
	contenido = "";
	
}

void Archivo::Escribir()
{

	std::ofstream flujoArchivo("Archivos/keyFramesGuardados.txt");
	if (!flujoArchivo)
	{
		// No se pudo abrir el archivo. Manejar el error.
		printf("\nOcurrio un error al abrir el archivo.\n");
		return;
	}

	flujoArchivo << contenido << std::endl;

	flujoArchivo.close();

}

void Archivo::LeerKeyFrames(std::string cadena)
{
	contenido.append(cadena + "\n");
}

void Archivo::LeerKeyFramesDeArchivo() {


    if (archivoEstaVacio()) {
        // L�gica para archivo vac�o

    }
    else {
        // L�gica para archivo con contenido
        std::ifstream archivo("Archivos/keyFramesGuardados.txt");
        std::string linea;

        //Expresiones regulares

        //Expresion regular para obtener numero indice entre corchetes
        std::regex regex("\\[([0-9]+)\\]");
        //Expresion regular para obtener valor de la coordenada
        //std::regex regex2("= (-?[0-9.]+)$");
        std::regex regex2("\\s*=\\s*(-?[0-9.]+)$");


        int indice = 0;
        float numeroFloat = 0.0f;
        int contador = 0;


        while (std::getline(archivo, linea)) {

            // Verificar si la l�nea est� vac�a
            if (linea.empty()) {
                break;
            }

            std::smatch coincidencias;
            std::smatch coincidencias2;

            // Utilizar std::regex_search para buscar el n�mero entre corchetes
            if (std::regex_search(linea, coincidencias, regex)) {
                // Obtener el n�mero de �ndice como una cadena
                std::string indiceStr = coincidencias[1].str();

                // Convertir la cadena a un n�mero entero (�ndice)
                indice = std::stoi(indiceStr);
            }

            // Utilizar std::regex_search para buscar el n�mero float al final de la l�nea
            if (std::regex_search(linea, coincidencias2, regex2)) {
                // Obtener el n�mero float como una cadena
                std::string floatStr = coincidencias2[1].str();

                // Convertir la cadena a un n�mero float
                numeroFloat = std::stof(floatStr);
            }

            // Aqu� se usa el indice y el float para ingresar en el KeyFrame
            if (contador == 0 and !linea.empty()) {
                KeyFrame[indice].movAnimacion_x = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].movAnimacion_x = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].mov_x = %f\n", indice, KeyFrame[indice].movAnimacion_x);
            }
            else if (contador == 1) {
                KeyFrame[indice].movAnimacion_y = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].movAnimacion_y = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].mov_y = %f\n", indice, KeyFrame[indice].movAnimacion_y);
            }
            else if (contador == 2) {
                KeyFrame[indice].movAnimacion_z = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].movAnimacion_z = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].mov_z = %f\n", indice, KeyFrame[indice].movAnimacion_z);
            }
            else if (contador == 3) {
                KeyFrame[indice].giroAnimacion = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].giroAnimacion = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].giro = %f\n", indice, KeyFrame[indice].giroAnimacion);
            }
            else if (contador == 4) {
                KeyFrame[indice].giroDosAnimacion = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].giroDosAnimacion = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].giroDos = %f\n", indice, KeyFrame[indice].giroDosAnimacion);
            }
            else if (contador == 5){
                KeyFrame[indice].giroTresAnimacion = numeroFloat;
                LeerKeyFrames("KeyFrame[" + std::to_string(indice) + "].giroTresAnimacion = " + std::to_string(numeroFloat));
                //printf("KeyFrame[%d].giroTres = %f\n", indice, KeyFrame[indice].giroTresAnimacion);
                contador = 0;
                FrameIndex ++;
                continue;
            }
            contador += 1;
        }
        i_curr_steps = FrameIndex;

        printf("\n\n\n");

        archivo.close();
    }

}

bool Archivo::archivoEstaVacio() {
    std::ifstream archivo("Archivos/keyFramesGuardados.txt");

    // Intentar leer una l�nea del archivo
    std::string linea;
    std::getline(archivo, linea);

    archivo.close();

    // Verificar si la l�nea le�da est� vac�a o si hubo un error al leer el archivo
    return linea.empty() || !archivo;
}

Archivo::~Archivo()
{
	
}