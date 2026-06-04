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
    //Punto 3
    int iTIPO_HOGAR_DCPOREDAD;
    //punto 4
    int iTIPO_HOGAR_DCTOTAL;
    //punto 5
    int iCUIDADO_SOLO_HOGAR;
    //punto 8
    int iTP_GRANGRUPO_PERSONALES;
    int iTCS_GRANGRUPO_TRABAJOTOTAL;
    int iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
    int iTCS_GRANGRUPO_TNR;
    int iTCS_GRANGRUPO_PERSONALES;

}RegIndice;

//punto 4
typedef struct
{
    int sumWHOG;
    int cantReg;
}Hogares;



typedef struct
{
    int ID;
    int WHOG;
    int WPER;
    int REGION;
    int EDAD_SEL;
    int SEXO_SEL;
    //punto 2, 6, 8
    char GRUPO_EDAD_SEL[20];//punto 2
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
    int TCS_GRANGRUPO_TRABAJOTOTAL;
    int TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
    int TCS_GRANGRUPO_TNR;
    int TCS_GRANGRUPO_PERSONALES;
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

//punto 8
typedef struct
{
    int id;
    int whog;
    int wper;
    int region;
    int sexo;
    char grupoEdad[50];
    char tipoTrabajo[50];
    int tiempo;
    int valor;
}tArchivoTiempo;



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

//punto 3
void DistribucionSegunDemadantesPorRegion(Registro* reg, int cant);
void mostrarDistribucionDemPorReg(int **mDemPorReg);
//punto 4
void calcularTotalesPorRegion(Registro *reg, int cant);
void mostrarTotalesPorRegion(Hogares hog[6][2]);
void calcularTotalesPorEdad(Registro *reg, int cant);
void mostrarTotalesPorEdad(const Hogares hog[6][3]);
void calcularProporciones(Registro *reg, int cant);
void mostrarProporciones(Hogares h[6][3],float prop[6][3],int *suma);
void mostrarProporcionesRegion(float p[6][3]);
//punto 5
void calcularDistribucionPorcentual(Registro *reg, int cant);
void mostrarDistribucionPorcentual(float prop[6][2]);
//punto 8
void construirArchTiempo(Registro *reg,int cant);
void mostrarArchivoTiempo(FILE *p);
//punto 9
void calcularTiempoProm();
void mostrarTiempoProm(float *prom,int ce);
//punto 10
void calcularTiempoPromSexo();
void mostrarTiempoPromSexo(float *promH,float *promM,int c,int f);
//punto 11
void calcularTiempoPromEdad();
int cmpRangoEdad(char *cmp);
void mostrarTiempoPromEdad(float prom[3][4],int c,int f);


//typedef int(*TxtAbin)(char*linea,void*reg);
//int ConversorCsvABin(char*archcsv,char*archbin,size_t tamreg,TxtAbin txtabin);
#endif // ARCHIVO_H_INCLUDED
