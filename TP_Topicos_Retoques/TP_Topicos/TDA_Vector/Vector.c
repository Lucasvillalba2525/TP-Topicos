#include "Vector.h"

bool vectorCrear(Vector* vector, size_t tamElem)
{
    vector->ce = 0;
    vector->cap = 0;
    vector->tamElem = 0;

    vector->vec = malloc(CAP_INI * tamElem);

    if(vector->vec == NULL)
    {
        return false;
    }

    vector->cap = CAP_INI;
    vector->tamElem = tamElem;

    return true;
}

void vectorEliminar(Vector*vector)
{
    vector->ce=0;
}

void vectorDestruir(Vector* vector)
{
    free(vector->vec);
    vector->vec = NULL;
    vector->cap = 0;
    vector->ce = 0;
    vector->tamElem = 0;
}
int vectorInsertarAlFinal(Vector* vector, const void* elem)
{
    if(vector->ce == vector->cap)
    {
        if(!ampliarVector(vector))
        {
            return ERROR_SIN_MEM;
        }
    }

    void* dirIns = vector->vec + vector->ce * vector->tamElem;
    memcpy(dirIns, elem, vector->tamElem);
    vector->ce++;

    return TODO_OK;
}

bool ampliarVector(Vector* vector)
{
    size_t nCap = vector->cap * AUMENTO;
    void* nVec = realloc(vector->vec, nCap * vector->tamElem);

    if(!nVec)
    {
        return false;
    }

    //printf("Ampliacion de %lld a %lld\n", vector->cap, nCap);

    vector->cap = nCap;
    vector->vec = nVec;

    return true;
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

int cmpentero(const void*a,const void*b)
{
    const int* e1=a;
    const int* e2=b;

    return *e1-*e2;
}

int cmppalabra(const void*pal1,const void*pal2)
{
    bool encontrado=true;
    const char*i=(char*)pal1;
    const char*c=(char*)pal2;
    while((*i != '\0' && *c != '\0') && encontrado)
    {
        if(*i==*c)
            encontrado=true;
        else
            encontrado=false;
        i++;
        c++;
    }
    if(encontrado)
        return ENCONTRADO;
    else
        return NO_ENCONTRADO;
}

int cmpfaltante(const void *pal1, const void *pal2)
{
    const char *a = (const char *)pal1;
    const char *b = (const char *)pal2;

    while(*a && *b && (*a == *b))
    {
        a++;
        b++;
    }

    if(*a == '\0' && *b == '\0')
        return ENCONTRADO;

    return NO_ENCONTRADO;
}
