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
    float movpapalote_x;       //Variable para PosicionX
    float movpapalote_y;       //Variable para PosicionY
    float movpapalote_z;       //Variable para PosicionZ
    float movpapalote_xInc;    //Variable para IncrementoX
    float movpapalote_yInc;    //Variable para IncrementoY
    float movpapalote_zInc;    //Variable para IncrementoZ
    float giropapalote;
    float giropapaloteInc;
} FRAME;

extern FRAME KeyFrame[MAX_FRAMES];

#endif


