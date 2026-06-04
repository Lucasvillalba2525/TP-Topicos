#include "..\Archivo\Archivo.h"

#define ARG_CSV 1

/*punto 4
Análisis de hogares según demandantes de cuidado y su distribución
porcentual por región
A) Totales por región

*/
int main(int argc, char* argv[])
{
    int cantReg = 0;

    Registro*reg=NULL;

    int ret=procesarCSV(argv[ARG_CSV], buscarpalabra, &reg,&cantReg);

    if(ret!=TODO_OK)
        printf("Error al Procesar archivo");

    printf("DEBUG FINAL cantReg=%d\n", cantReg);

    //mostrarRegistros(reg, cantReg);

    //punto 1
    //SumaCantidad(&reg,cantReg);

    //punto 2
    //clasificacionRangoEtario(reg,21);

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

    //punto 8
    //construirArchTiempo(reg,cantReg);
    //free(reg);
    //punto 9
    //calcularTiempoProm();
    //punto 10
    //calcularTiempoPromSexo();
    //punto 11
    calcularTiempoPromEdad();

    return 0;
}

bool buscarpalabra(void*pal1,void*pal2)
{
    bool encontrado=true;
    char*i=(char*)pal1;
    char*c=(char*)pal2;

    while((*i != '\0' && *c != '\0') && encontrado)
    {
        if(*i!=*c)
            encontrado=false;
        i++;
        c++;
    }
    return encontrado;
}

bool buscarfaltante(void*pal1,void*pal2)
{
    bool encontrado=true;
    char*i=(char*)pal1;
    char*c=(char*)pal2;

    while(*i!=' ' && encontrado)
    {
        if(*i==*c)
            encontrado=true;
        else
            encontrado=false;
        i++;
        c++;
    }

    if(encontrado)
        return true;
    else
        return false;

}

