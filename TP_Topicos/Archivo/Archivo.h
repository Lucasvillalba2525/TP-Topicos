#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include "..\TDA_Vector\Vector.h"

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
    //punto 1
    int iID;
    int iWHOG;
    int iWPER;
    int iREGION;
    int iEDAD_SEL;
    int iSEXO_SEL;
    int iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
    int iTP_GRANGRUPO_TRABAJOTOTAL;
    int iTP_GRANGRUPO_TNR;

}RegIndice;

typedef struct
{
    int ID;
    int WHOG;
    int WPER;
    int REGION;
    int EDAD_SEL;
    int SEXO_SEL;
    //punto 2, 6, 8
    char GRUPO_EDAD_SEL[20];
    int TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
    int TP_GRANGRUPO_TRABAJOTOTAL;
    int TP_GRANGRUPO_TNR;
    //punto 3
    int TIPO_HOGAR_DCPOREDAD;
    //punto 4
    int TIPO_HOGAR_DCTOTAL;
    //punto 5
    int CUIDADO_SOLO_HOGAR;
    //punto 7
    int GRUPO_EDAD;
    //punto 8
    int TP_GRANGRUPO_PERSONALES;
    //punto 9


}Registro;

typedef struct
{
    int ID;
    char Nombre_Region[TAM_PAL];
    int CantReg;
    int CantEstHogares;
    int CantEstPersonas;
}CantPorRegion;

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
void mostrarRegistrosIndices(RegIndice* iReg, int cantReg);
void mostrarRangoEtarios(Registro *reg,int cant);
void clasificacionRangoEtario(Registro *reg,int cant);
bool buscarpalabra(void*pal1,void*pal2);
bool buscarfaltante(void*pal1,void*pal2);


//typedef int(*TxtAbin)(char*linea,void*reg);
//int ConversorCsvABin(char*archcsv,char*archbin,size_t tamreg,TxtAbin txtabin);
#endif // ARCHIVO_H_INCLUDED
