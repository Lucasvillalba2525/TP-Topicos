#include "C:\Users\Argentech\Desktop\TP_Topicos\TDA_Vector\Vector.h"

int vectorCrear(Vector*vector)
{
    vector->vec=malloc(MAX_LINEA*sizeof(int));
    if(!vector->vec)
        {
            vector->cap=0;
            return ERROR_SIN_MEM;
        }

    vector->cap=MAX_LINEA;
    vector->ce=0;
   return TODO_OK;
}

void vectorEliminar(Vector*vector)
{
    vector->ce=0;
}

void vectorDestruir(Vector*vector)
{
    free(vector->vec);
    vector->vec=NULL;
    vector->cap=MAX_LINEA;
    vector->ce=0;

}

int vectorInsertarAlFinal(Vector*vector,int elem)
{
    if(vector->cap==vector->ce)
    {
        if(Redimensionar(vector,AUMENTO)==-1)
            return ERROR_SIN_MEM;
    }

    int *dirIns=vector->vec + vector->ce;
    *dirIns=elem;
    vector->ce++;

    return TODO_OK;
}

int Redimensionar(Vector*vector,int factor)
{
    int*nuevovec=realloc(vector->vec,sizeof(int)*(vector->cap*factor));
    if(!nuevovec)
        {
            printf("FALLO REALLOC\n");
            return ERROR_SIN_MEM;
        }

    vector->vec=nuevovec;
    vector->cap*=factor;

    return TODO_OK;
}

void vectorMostrar(Vector*vector)
{
    int*j=vector->vec + (vector->ce-1);

    for(int*i=vector->vec;i<j;i++)
        printf("[%d]",*i);

}
