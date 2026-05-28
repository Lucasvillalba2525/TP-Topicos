#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#define AUMENTO 2
#define TODO_OK 1
#define ERROR_SIN_MEM -1
#define MAX_LINEA 8192


typedef struct
{
    int*vec;
    size_t cap;
    size_t ce;

}Vector;

int vectorCrear(Vector*vector);
int vectorInsertarAlFinal(Vector*vector,int elem);
void vectorEliminar(Vector*vector);
void vectorDestruir(Vector*vector);
int Redimensionar(Vector*vector,int factor);
void vectorMostrar(Vector*vector);

#endif // VECTOR_H_INCLUDED
