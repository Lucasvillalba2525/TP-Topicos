#include"C:\Users\Argentech\Desktop\TP_Topicos\Archivo\Archivo.h"
#define esletra(c)((c>='A' && c<='Z')||(c>='a' && c<='z'))
//#define esguion(c) (c=='_')
#define esComilla(c) (c=='"')
#define esEspacio(c) (c==' ')

void secpalCrear(char*Cad,Secpal*sec)
{
    sec->cursor=Cad;
    sec->finsec=false;
}
bool secpalFin(Secpal*sec)
{
    if(sec->finsec==true)
        return true;
    else
        return false;
}
int secpalLeer(Secpal*sec,Palabra*pal)
{
    while(*sec->cursor!='\0' && (esComilla(*sec->cursor)||esEspacio(*sec->cursor)))
        sec->cursor++;

    if(*sec->cursor=='\0')
    {
        sec->finsec=true;
        return FIN;
    }
    char*ipal=pal->vpal;

    while(*sec->cursor!='\0' && !esComilla(*sec->cursor))
    {
        *ipal=*sec->cursor;
        ipal++;
        sec->cursor++;
    }

    *ipal='\0';

    return TODO_OK;
}

bool buscar(Palabra*pal,char*campo)
{
    bool encontrado=true;
    char*i=pal->vpal;
    char*c=campo;

    while(*i!='\0' && encontrado)
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

int vectorInsertarAlFinal(Vector*vector,char*linea)
{
   int*i=vector->vec+(vector->ce-1);
   char*cursor=linea;

   if(vector->cap==vector->ce)
        redimensionar(vector);

    while(cursor!=NULL)
    {
        *i=atoi(cursor);
        cursor++;
        i++;
        vector->ce++;
    }

    return TODO_OK;
}

int redimensionar(Vector*vec)
{
    return TODO_OK;
}
