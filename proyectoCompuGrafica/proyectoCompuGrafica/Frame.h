#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>


#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_FRAMES 50
extern int i_max_steps;
extern int i_curr_steps;
extern int FrameIndex;
extern bool play;
extern int playIndex;

typedef struct _frame
{
    //guardar en pares (complementaria)

    //Variables para GUARDAR Key Frames
    float movAnimacion_x;       //Variable para PosicionX
    float movAnimacion_y;       //Variable para PosicionY
    float movAnimacion_z;       //Variable para PosicionZ
    float movAnimacion_xInc;    //Variable para IncrementoX
    float movAnimacion_yInc;    //Variable para IncrementoY
    float movAnimacion_zInc;    //Variable para IncrementoZ
    float giroAnimacion;
    float giroAnimacionInc;

    float giroDosAnimacion;     //Variable para Posicion de segundo eje
    float giroDosAnimacionInc;  //Variable para Incremento en un segundo eje

    float giroTresAnimacion;    //Variable para Posicion de tercer eje
    float giroTresAnimacionInc; //Variable para Incremento en un tercer eje
} FRAME;

extern FRAME KeyFrame[MAX_FRAMES];

#endif


