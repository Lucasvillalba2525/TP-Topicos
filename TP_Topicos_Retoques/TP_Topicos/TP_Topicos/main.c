#include "..\Archivo\Archivo.h"

#define ARG_CSV 1

//punteros a funcion del punto 6 y 7
int obtenerFilaSexo(Registro *r);
int obtenerFilaEdad(Registro *r);
void acumSexo(int **mat, int fila, int col, int dato);
void acumEdad(int **mat, int fila, int col, int dato);



/*punto 4
Análisis de hogares según demandantes de cuidado y su distribución
porcentual por región
A) Totales por región

*/
int main(int argc, char* argv[])
{
    int cantReg = 0;

    Registro*reg=NULL;
    Vector vecInd;

    vectorCrear(&vecInd,sizeof(Indice));


    int ret=procesarCSV(argv[ARG_CSV],cmppalabra,&reg,&cantReg,&vecInd);

    if(ret!=TODO_OK)
        printf("Error al Procesar archivo");

    printf("DEBUG FINAL cantReg=%d\n", cantReg);

    //mostrarRegistros(reg, cantReg);

    putchar('\n');

    //punto 1
    //SumaCantidad(&reg,cantReg);

    putchar('\n');

    //punto 2
    clasificacionRangoEtario(reg,cantReg);

    putchar('\n');

    //Punto 3
    //DistribucionSegunDemadantesPorRegion(reg, cantReg);

    //punto 4
    //A) Totales por región
    //calcularTotalesPorRegion(reg, cantReg);
    //B) Distribución por edad de demandantes
    //calcularTotalesPorEdad(reg, cantReg);
    //mostrarTotalesPorEdad(const Hogares hogares[6][3]);
    //C) Calculo de Proporciones
    //calcularProporciones(reg,cantReg);

    //punto 5
    //calcularDistribucionPorcentual(reg,cantReg);

    //punto6
   int** matSexo=(int**)crearMatriz(SEXOYTOT,TIPOTRABAJO,sizeof(int));
    if(!matSexo)
        {
            printf("Error al crear Matriz");
            return -1;
        }

    int vecAcumSexo[2]={0}; //hom, muj

    int CantPer=Sumadeponderadores(reg,cantReg,matSexo,TIPOTRABAJO,obtenerFilaSexo,acumSexo,vecAcumSexo);

    char *FilasSexo[]={"SEXO_SEL","Mujer","Hombre","Total"};

    float**matPorcSexo=(float**)crearMatriz(SEXOYTOT,TIPOTRABAJO,sizeof(float));
    if(!matPorcSexo)
        return ERROR_SIN_MEM;

    calcularPorcentaje(matSexo,SEXOYTOT,TIPOTRABAJO,matPorcSexo,CantPer,vecAcumSexo,FilasSexo);

    putchar('\n');

    guardarMatrizBin("MatrizSexo.bin",matPorcSexo,SEXOYTOT,TIPOTRABAJO);

    leerMatrizBin("MatrizSexo.bin", SEXOYTOT,TIPOTRABAJO,FilasSexo);

    //punto7
    int** matEdad=(int**)crearMatriz(GRUPOEDAD,TIPOTRABAJO,sizeof(int));
    if(!matEdad)
        {
            printf("Error al crear Matriz");
            return -1;
        }

    int vecAcumEdad[3]={0};

    CantPer=Sumadeponderadores(reg,cantReg,matEdad,GRUPOEDAD,obtenerFilaEdad,acumEdad,vecAcumEdad);

    char *FilasEdad[]={"GRUPO_EDAD_SEL","14 a 29 anios","30 a 64 anios","65 anios y mas","Total"};

    float**matPorcEdad=(float**)crearMatriz(GRUPOEDAD,TIPOTRABAJO,sizeof(float));
    if(!matPorcEdad)
        return ERROR_SIN_MEM;

    calcularPorcentaje(matEdad,GRUPOEDAD,TIPOTRABAJO,matPorcEdad,CantPer,vecAcumEdad,FilasEdad);

    putchar('\n');

    guardarMatrizBin("MatrizEdad.bin",matPorcEdad,GRUPOEDAD,TIPOTRABAJO);

    leerMatrizBin("MatrizEdad.bin", GRUPOEDAD,TIPOTRABAJO,FilasEdad);

    destruirMatriz((void**)matSexo,SEXOYTOT);

    destruirMatriz((void**)matEdad,GRUPOEDAD);

    destruirMatriz((void**)matPorcSexo,SEXOYTOT);

    destruirMatriz((void**)matPorcEdad,GRUPOEDAD);


    //punto 8
    //construirArchTiempo(reg,cantReg);
    //free(reg);
    //punto 9
    //calcularTiempoProm();
    //punto 10
    //calcularTiempoPromSexo();
    //punto 11
    calcularTiempoPromEdad();

    vectorDestruir(&vecInd);

    return 0;
}

//Abajo todo del ejercicio 6 y 7
int obtenerFilaSexo(Registro *r)
{
    return r->SEXO_SEL - 1;
}

int obtenerFilaEdad(Registro *r)
{
    if (r->EDAD_SEL >= 14 && r->EDAD_SEL <= 29)
        return 0;

    if (r->EDAD_SEL >= 30 && r->EDAD_SEL <= 64)
        return 1;

    return 2;
}

void acumSexo(int **mat, int fila, int col, int dato)
{
    mat[fila][col]+=dato;  // sexo

    mat[2][col]+=dato;     // total

}
void acumEdad(int **mat, int fila, int col, int dato)
{
    mat[fila][col]+=dato;  // sexo

    mat[3][col]+=dato;     // total

}

