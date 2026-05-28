#include"C:\Users\Argentech\Desktop\TP_Topicos\Archivo\Archivo.h"

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
    while(*sec->cursor != '\0' && (*sec->cursor == ' ' || *sec->cursor == '\t' || *sec->cursor == '\n' || *sec->cursor == '"'))
        sec->cursor++;

    if(*sec->cursor=='\0')
    {
        sec->finsec=true;
        return FIN;
    }
    char*ipal=pal->vpal;

    while(*sec->cursor != '\0' && *sec->cursor != ' ' && *sec->cursor != '\t' && *sec->cursor != '\n' && *sec->cursor != '"')
    {
        *ipal=*sec->cursor;
        ipal++;
        sec->cursor++;
    }

    *ipal='\0';

    return TODO_OK;
}

int procesarCSV(const char *arch,Buscar buscar,Registro **reg,int *cantReg)
{
    FILE *fp = fopen(arch, "r");

    if(!fp)
        return ERROR_ARCHIVO;

    char linea[MAX_LINEA];

    // leer encabezado
    if(!fgets(linea, sizeof(linea), fp))
    {
        fclose(fp);
        return FALLA_LEC;
    }

    // detectar índices
    Secpal seclec;
    Palabra pal;

    int iID=-1, iWHOG=-1, iWPER=-1,iREGION=-1, iSEXO=-1, iEDAD=-1;
    int cont=0;

    secpalCrear(linea, &seclec);
    secpalLeer(&seclec, &pal);

    while(!secpalFin(&seclec))
    {
        if(buscar(&pal,"ID"))iID = cont;
        if(buscar(&pal,"WHOG"))iWHOG = cont;
        if(buscar(&pal,"WPER"))iWPER = cont;
        if(buscar(&pal,"REGION"))iREGION = cont;
        if(buscar(&pal,"EDAD_SEL"))iEDAD = cont;
        if(buscar(&pal,"SEXO_SEL"))iSEXO = cont;

        secpalLeer(&seclec, &pal);
        cont++;
    }

    RegIndice indreg =
    {
        iID,
        iWHOG,
        iWPER,
        iREGION,
        iEDAD,
        iSEXO
    };

    int cap = 10;

    Registro *miReg = malloc(cap * sizeof(Registro));

    if(!miReg)
    {
        fclose(fp);
        return ERROR_SIN_MEM;
    }

    Vector miVec;
    vectorCrear(&miVec);

    *cantReg = 0;

    while(fgets(linea, MAX_LINEA, fp))
    {
        parseolinea(linea, &miVec, buscar);

        if(*cantReg == cap)
        {
            cap *= 2;

            Registro *tmp =realloc(miReg, cap * sizeof(Registro));

            if(!tmp)
            {
                free(miReg);
                vectorDestruir(&miVec);
                fclose(fp);

                return ERROR_SIN_MEM;
            }

            miReg = tmp;
        }

        procesarDatos(&miVec,miReg,*cantReg,&indreg);

        (*cantReg)++;

        vectorEliminar(&miVec);
    }

    vectorDestruir(&miVec);
    fclose(fp);

    *reg = miReg;

    return TODO_OK;
}
void parseolinea(char *linea, Vector *vector, Buscar buscar)
{
    Secpal seclec;
    Palabra pal;

    secpalCrear(linea, &seclec);
    secpalLeer(&seclec, &pal);

    while(!secpalFin(&seclec))
    {
        int dato;

        if (buscarfaltante(&pal, "NA"))
            dato = DATO_FALT;
        else
            dato = atoi(pal.vpal);

        if (vectorInsertarAlFinal(vector, dato) != TODO_OK)
        {
            printf("ERROR INSERT VECTOR\n");
            return;
        }

        secpalLeer(&seclec, &pal);
    }
}

void procesarDatos(Vector *vector, Registro *reg,int cantReg, RegIndice *ireg)
{
    int *v = vector->vec;

    reg[cantReg].ID       = v[ireg->iID];

    reg[cantReg].WHOG     = v[ireg->iWHOG];

    reg[cantReg].WPER     = v[ireg->iWPER];

    reg[cantReg].REGION   = v[ireg->iREGION];

    reg[cantReg].EDAD_SEL = v[ireg->iEDAD_SEL];

    reg[cantReg].SEXO_SEL = v[ireg->iSEXO_SEL];
}

void mostrarRegistros(Registro* Reg, int cantReg)
{
    puts("");

    for(int i = 0; i < cantReg; i++)
    {
        printf("Nro Reg: %d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i,
               Reg[i].ID,
               Reg[i].WHOG,
               Reg[i].WPER,
               Reg[i].REGION,
               Reg[i].EDAD_SEL,
               Reg[i].SEXO_SEL);

        fflush(stdout);
    }
}

void SumaCantidad(Registro **miReg, int cantRegisTot)
{
    CantPorRegion Region[REGIONES] = {
        {1,"GBA",0,0,0},
        {2,"PAMPEANA",0,0,0},
        {3,"NOROESTE",0,0,0},
        {4,"NORESTE",0,0,0},
        {5,"CUYO",0,0,0},
        {6,"PATAGONIA",0,0,0}
    };

    Registro *p = *miReg;

    for(int i = 0; i < cantRegisTot; i++, p++)
    {
        int r = p->REGION;

        if(r >= 1 && r <= REGIONES)
        {
            (Region + (r - 1))->CantEstHogares += p->WHOG;
            (Region + (r - 1))->CantEstPersonas += p->WPER;
            (Region + (r - 1))->CantReg++;
        }
    }

    printf("REGION\tcant_registros\tcant_hogares_est\tcant_personas_est\tnombre.region\n");

    for(int i = 0; i < REGIONES; i++)
    {
        printf("%d\t%8d\t%8d\t%8d\t%s\n",
               (Region + i)->ID,
               (Region + i)->CantReg,
               (Region + i)->CantEstHogares,
               (Region + i)->CantEstPersonas,
               (Region + i)->Nombre_Region);
    }
}

