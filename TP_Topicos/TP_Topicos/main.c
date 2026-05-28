#include"C:\Users\Argentech\Desktop\TP_Topicos\Archivo\Archivo.h"

#define ARG_CSV 1

int main(int argc, char* argv[])
{
    int cantReg = 0;

    Registro*reg=NULL;

    int ret=procesarCSV(argv[ARG_CSV], buscarpalabra, &reg,&cantReg);

    if(ret!=TODO_OK)
        printf("Error al Procesar archivo");

    printf("DEBUG FINAL cantReg=%d\n", cantReg);

    //mostrarRegistros(Registro**reg, cantReg);

    SumaCantidad(&reg,cantReg);

    free(reg);

    return 0;
}

bool buscarpalabra(void*pal1,void*pal2)
{
    bool encontrado=true;
    char*i=(char*)pal1;
    char*c=(char*)pal2;

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
        return true;
    else
        return false;

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

/*













 ================= DATA =================
   while (fgets(line, sizeof(line), fp))
    {

        Registro r = {0};

        char *cols[MAX_COLS];
        int c = parse_line(line, delim, cols);

        for (int i = 0; i < c; i++) {

            if (i == iID)     strcpy(r.id, cols[i]);
            if (i == iWHOG)   strcpy(r.whog, cols[i]);
            if (i == iWPER)   strcpy(r.wper, cols[i]);
            if (i == iREGION) strcpy(r.region, cols[i]);
            if (i == iSEXO)   strcpy(r.sexo_sel, cols[i]);
            if (i == iEDAD)   strcpy(r.edad_sel, cols[i]);
        }

        printf("ID=%s REGION=%s SEXO=%s EDAD=%s\n",
               r.id, r.region, r.sexo_sel, r.edad_sel);
    }

    fclose(fp);

*/

/*
void parseolinea(char* linea,Vector*vector,Buscar buscar)
{
    char*i=linea;
    int dato,cont;
    bool encontrado;
    while(*i!='\0')
    {
        encontrado=false;

        if(!esEspacio(*i))
        {
            cont=0;
            if(buscarfaltante(i,"NA")==true)
            {
                dato=DATO_FALT;
                if(vectorInsertarAlFinal(vector, dato) != TODO_OK)
                {
                    printf("ERROR INSERT VECTOR\n");
                    break;
                }
                i=i+2;
                encontrado=true;
            }
            else
            {
                dato=atoi(i);
                if(vectorInsertarAlFinal(vector, dato) != TODO_OK)
                {
                    printf("ERROR INSERT VECTOR\n");
                    break;
                }

                if(dato==0)
                    cont++;

                while(dato>0)//Ver si el dato es cero
                {
                    dato=dato/10;
                    cont++;
                }

                encontrado=true;
                i=i+cont;
            }

        }

        if(!encontrado)
            i++;
    }

}
*/

/*
void parseolinea(char *linea, Vector *vector, Buscar buscar)
{
    char *p = linea;

    while (*p != '\0')
    {
        // saltar espacios
        while (*p == ' ' || *p == '\t' || *p == '\n')
            p++;

        if (*p == '\0')
            break;

        // inicio del token
        char *inicio = p;

        // avanzar hasta fin del token
        while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
            p++;

        // guardar fin temporal
        char c = *p;
        *p = '\0';

        int dato;

        if (buscarfaltante(inicio, "NA"))
            dato = DATO_FALT;
        else
            dato = atoi(inicio);

        if (vectorInsertarAlFinal(vector, dato) != TODO_OK)
        {
            printf("ERROR INSERT VECTOR\n");
            *p = c;
            return;
        }

        // restaurar carácter
        *p = c;
    }
}
*/

