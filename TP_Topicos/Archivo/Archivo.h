#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#define ERROR_SIN_MEM -1
#define ERROR_ARCHIVO -1
#define FALLA_LEC -1
#define FIN 0
#define TODO_OK 1
#define CANT_CAMPOS 6
#define MAX_LINEA 8192
#define TAM_PAL 100

typedef struct
{
    int*vec;
    size_t cap;
    size_t ce;

}Vector;

typedef struct
{
    int iID;
    int iWHOG;
    int iWPER;
    int iREGION;
    int iSEXO_SEL;
    int iEDAD_SEL;
} RegIndice;

typedef struct
{
    int id;
    int whog;
    int wper;
    int region;
    int sexo_sel;
    int edad_sel;
} Registro;

typedef struct
{
    char*cursor;
    bool finsec;
}Secpal;

typedef struct
{
    char vpal[TAM_PAL];
}Palabra;

typedef int(*TxtAbin)(char*linea,void*reg);

int ConversorCsvABin(char*archcsv,char*archbin,size_t tamreg,TxtAbin txtabin);
int procesarCSV(const char *archivo);
void secpalCrear(char*Cad,Secpal*sec);
int secpalLeer(Secpal*sec,Palabra*pal);
bool secpalFin(Secpal*sec);
bool buscar(Palabra*pal,char*campo);
int vectorCrear(Vector*vector);
int vectorInsertarAlFinal(Vector*vector,char*linea);
int redimensionar(Vector*vec);
#endif // ARCHIVO_H_INCLUDED
