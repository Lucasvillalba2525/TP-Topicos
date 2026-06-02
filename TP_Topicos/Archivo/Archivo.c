#include "Archivo.h"

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

    // detectar �ndices
    Secpal seclec;
    Palabra pal;

    int iID=-1, iWHOG=-1, iWPER=-1,iREGION=-1, iSEXO=-1, iEDAD=-1,iOCUPYAUTO=-1,iTRABTOTAL=-1,iTNR=-1,iTIPO_HOGAR_DCPOREDAD=-1;
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
        if(buscar(&pal,"TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO"))iOCUPYAUTO = cont;
        if(buscar(&pal,"TP_GRANGRUPO_TRABAJOTOTAL"))iTRABTOTAL = cont;
        if(buscar(&pal,"TP_GRANGRUPO_TNR"))iTNR = cont;
        if(buscar(&pal,"TIPO_HOGAR_DCPOREDAD"))iTIPO_HOGAR_DCPOREDAD = cont;

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
        iSEXO,
        iOCUPYAUTO,
        iTRABTOTAL,
        iTNR,
        iTIPO_HOGAR_DCPOREDAD
    };

    //mostrarRegistrosIndices(&indreg,9);
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

    reg[cantReg].TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO = v[ireg->iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO];

    reg[cantReg].TP_GRANGRUPO_TRABAJOTOTAL = v[ireg->iTP_GRANGRUPO_TRABAJOTOTAL];

    reg[cantReg].TP_GRANGRUPO_TNR = v[ireg->iTP_GRANGRUPO_TNR];

    reg[cantReg].TIPO_HOGAR_DCPOREDAD = v[ireg->iTIPO_HOGAR_DCPOREDAD];
}

void mostrarRegistrosIndices(RegIndice *iReg, int cantReg)
{
    puts("");
    printf("%d %d %d %d %d %d %d %d %d %d\n",
               iReg->iID,
               iReg->iWHOG,
               iReg->iWPER,
               iReg->iREGION,
               iReg->iEDAD_SEL,
               iReg->iSEXO_SEL,
               iReg->iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO,
               iReg->iTP_GRANGRUPO_TRABAJOTOTAL,
               iReg->iTP_GRANGRUPO_TNR,
               iReg->iTIPO_HOGAR_DCPOREDAD);

    fflush(stdout);
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

void clasificacionRangoEtario(Registro *reg,int cant)
{
    for(int i = 0; i < cant; i++)
    {
        if(reg[i].EDAD_SEL >= 14 && reg[i].EDAD_SEL <= 29)
            strcpy(reg[i].GRUPO_EDAD_SEL,"14 a 29 anios");
        if(reg[i].EDAD_SEL >= 30 && reg[i].EDAD_SEL <= 64)
            strcpy(reg[i].GRUPO_EDAD_SEL,"30 a 64 anios");
        if(reg[i].EDAD_SEL >= 65)
            strcpy(reg[i].GRUPO_EDAD_SEL,"65 anios o mas");
    }

    mostrarRangoEtarios(reg,cant);
}

void mostrarRangoEtarios(Registro *reg,int cant)
{
    printf("ID\tWHOG\tWPER\tREGION\tSEXO_SEL\tEDAD_SEL\tOCUPACIONYAUTO\tTRAB.TOTAL\tTNR\tGRUPO_EDAD_SEL\n");

    for(int i = 0; i < 10; i++)
    {
        printf("%d\t%15d\t%15d\t%20d\t%15d\t%15d\t%15d\t%15d\t%15d\t%s\n",
               (reg + i)->ID,
               (reg + i)->WHOG,
               (reg + i)->WPER,
               (reg + i)->REGION,
               (reg + i)->SEXO_SEL,
               (reg + i)->EDAD_SEL,
               (reg + i)->TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO,
               (reg + i)->TP_GRANGRUPO_TRABAJOTOTAL,
               (reg + i)->TP_GRANGRUPO_TNR,
               (reg + i)->GRUPO_EDAD_SEL);
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
        printf("%d\t%15d\t%15d\t%20d\t\t%s\n",
               (Region + i)->ID,
               (Region + i)->CantReg,
               (Region + i)->CantEstHogares,
               (Region + i)->CantEstPersonas,
               (Region + i)->Nombre_Region);
    }
}

//Punto 3
void DistribucionSegunDemadantesPorRegion(Registro* reg, int cant)
{
    //ideal en una libreria estatica
    int **mDemPorReg = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        mDemPorReg[i] = (int *)calloc(6, sizeof(int));
    }

    for(int i=0; i<cant; i++)
    {

        mDemPorReg[reg->TIPO_HOGAR_DCPOREDAD][reg->REGION-1]+=reg->WHOG;

        reg++;
    }

    mostrarDistribucionDemPorReg(mDemPorReg);

    //Limpio matriz
    for (int i = 0; i < 3; i++)
    {
        free(mDemPorReg[i]);
    }
    free(mDemPorReg);
}

void mostrarDistribucionDemPorReg(int **mDemPorReg)
{
    const char *regiones[] = {"GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA"};
    const char *tipoHogarDesc[] = {"Solo hasta 13 anios", "Solo 14 y mas", "Ambos tipos", "Sin demandantes"};

    // Orden de visualización: filas 1,2,3 primero, fila 0 al final
    int orden[] = {1, 2, 3, 0};

    // 1. Encabezado de las regiones
    printf("%-25s", "");
    for(int i = 0; i < 6; i++)
    {
        printf("%-12s", *(regiones + i));
    }

    printf("\n----------------------------------------------------------------------------------------------\n");

    // 2. Recorrido de la matriz usando el orden definido
    for(int i = 0; i < 4; i++)
    {
        int fila = *(orden + i); // índice real en la matriz

        printf("%-25s", *(tipoHogarDesc + fila));
        for(int j = 0; j < 6; j++)
        {
            printf("%-12d", mDemPorReg[fila][j]);
        }
        putchar('\n');
    }
}


