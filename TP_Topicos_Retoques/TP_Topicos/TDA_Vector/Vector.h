#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define AUMENTO 1.5
#define TODO_OK 1
#define ERROR_SIN_MEM -1
#define MAX_LINEA 8192
#define ENCONTRADO 0
#define NO_ENCONTRADO -1
#define CAP_INI 100

typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;

typedef int(*Cmp)(const void* a, const void* b);

bool vectorCrear(Vector* vector, size_t tamElem);
int vectorInsertarAlFinal(Vector* vector, const void* elem);
void vectorEliminar(Vector*vector);
void vectorDestruir(Vector*vector);
int Redimensionar(Vector*vector,int factor);
void vectorMostrar(Vector*vector);
bool ampliarVector(Vector* vector);

int cmpentero(const void*a,const void*b);
int cmppalabra(const void*pal1,const void*pal2);
int cmpfaltante(const void*pal1,const void*pal2);

#endif // VECTOR_H_INCLUDED
