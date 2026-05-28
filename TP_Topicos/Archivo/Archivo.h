#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include "C:\Users\Argentech\Desktop\TP_Topicos\TDA_Vector\Vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define REGIONES 6
#define ERROR_SIN_MEM -1
#define ERROR_ARCHIVO -1
#define FALLA_LEC -1
#define FIN 0
#define TODO_OK 1
#define MAX_LINEA 8192
#define TAM_PAL 100
#define DATO_FALT -1


typedef struct
{
    int iID;
    int iWHOG;
    int iWPER;
    int iREGION;
    int iEDAD_SEL;
    int iSEXO_SEL;
} RegIndice;

typedef struct
{
    int ID;
    int WHOG;
    int WPER;
    int REGION;
    int EDAD_SEL;
    int SEXO_SEL;
}
 Registro;

typedef struct
{
    int ID;
    char Nombre_Region[TAM_PAL];
    int CantReg;
    int CantEstHogares;
    int CantEstPersonas;
}
CantPorRegion;

typedef struct
{
    char*cursor;
    bool finsec;
}Secpal;

typedef struct
{
    char vpal[TAM_PAL];
}Palabra;


typedef bool(*Buscar)(void*pal,void*linea);

void secpalCrear(char*Cad,Secpal*sec);
int secpalLeer(Secpal*sec,Palabra*pal);
bool secpalFin(Secpal*sec);
int procesarCSV(const char *arch,Buscar buscar,Registro **reg,int *cantReg);
void parseolinea(char* linea,Vector*vector,Buscar buscar);
void procesarDatos(Vector*vector,Registro*reg,int cantReg,RegIndice*ireg);
void mostrarRegistros(Registro*Reg,int cantReg);
void SumaCantidad(Registro**miReg,int cantRegTot);


bool buscarpalabra(void*pal1,void*pal2);
bool buscarfaltante(void*pal1,void*pal2);


//typedef int(*TxtAbin)(char*linea,void*reg);
//int ConversorCsvABin(char*archcsv,char*archbin,size_t tamreg,TxtAbin txtabin);
#endif // ARCHIVO_H_INCLUDED
