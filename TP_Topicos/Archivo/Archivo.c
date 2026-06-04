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

    int iID=-1, iWHOG=-1, iWPER=-1,iREGION=-1, iSEXO=-1, iEDAD=-1,iOCUPYAUTO=-1,
    iTRABTOTAL=-1,iTNR=-1,iTIPO_HOGAR_DCPOREDAD=-1,iTIPO_HOGAR_DCTOTAL=-1,
    iCUIDADO_SOLO_HOGAR=-1,iTP_GRANGRUPO_PERSONALES=-1,iTCS_GRANGRUPO_TRABAJOTOTAL=-1,
    iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO=-1,iTCS_GRANGRUPO_TNR=-1,iTCS_GRANGRUPO_PERSONALES=-1;
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
        if(buscar(&pal,"TIPO_HOGAR_DCTOTAL"))iTIPO_HOGAR_DCTOTAL = cont;
        if(buscar(&pal,"CUIDADO_SOLO_HOGAR"))iCUIDADO_SOLO_HOGAR = cont;
        if(buscar(&pal,"TP_GRANGRUPO_PERSONALES"))iTP_GRANGRUPO_PERSONALES = cont;
        if(buscar(&pal,"TCS_GRANGRUPO_TRABAJOTOTAL"))iTCS_GRANGRUPO_TRABAJOTOTAL = cont;
        if(buscar(&pal,"TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO"))iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO = cont;
        if(buscar(&pal,"TCS_GRANGRUPO_TNR"))iTCS_GRANGRUPO_TNR = cont;
        if(buscar(&pal,"TCS_GRANGRUPO_PERSONALES"))iTCS_GRANGRUPO_PERSONALES = cont;


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
        iTIPO_HOGAR_DCPOREDAD,
        iTIPO_HOGAR_DCTOTAL,
        iCUIDADO_SOLO_HOGAR,
        iTP_GRANGRUPO_PERSONALES,
        iTCS_GRANGRUPO_TRABAJOTOTAL,
        iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO,
        iTCS_GRANGRUPO_TNR,
        iTCS_GRANGRUPO_PERSONALES
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

    reg[cantReg].TIPO_HOGAR_DCTOTAL = v[ireg->iTIPO_HOGAR_DCTOTAL];

    reg[cantReg].CUIDADO_SOLO_HOGAR = v[ireg->iCUIDADO_SOLO_HOGAR];

    reg[cantReg].TP_GRANGRUPO_PERSONALES = v[ireg->iTP_GRANGRUPO_PERSONALES];

    reg[cantReg].TCS_GRANGRUPO_TRABAJOTOTAL = v[ireg->iTCS_GRANGRUPO_TRABAJOTOTAL];

    reg[cantReg].TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO = v[ireg->iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO];

    reg[cantReg].TCS_GRANGRUPO_TNR = v[ireg->iTCS_GRANGRUPO_TNR];

    reg[cantReg].TCS_GRANGRUPO_PERSONALES = v[ireg->iTCS_GRANGRUPO_PERSONALES];

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
//punto 2
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

    for(int i = 0; i < cant; i++)
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
//punto 1
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

//punto 4
//A) Totales por región
void calcularTotalesPorRegion(Registro *reg, int cant)
{
    Hogares hog[6][2]= {0};
    Registro *aux=reg;

    for(int i=0; i<cant;i++,aux++)
    {
        hog[aux->REGION-1][aux->TIPO_HOGAR_DCTOTAL].sumWHOG+=aux->WHOG;
        hog[aux->REGION-1][aux->TIPO_HOGAR_DCTOTAL].cantReg++;
    }

    mostrarTotalesPorRegion(hog);
}
void mostrarTotalesPorRegion(Hogares hog[6][2])
{
    printf("REGION\tTIPO_HOGAR_DCTOTAL\tCantidad_Registro\tCantidad_Hog_Estimado\n");

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            printf("%d\t%15d\t%20d\t%20d\n", i+1, j, hog[i][j].cantReg, hog[i][j].sumWHOG);
        }
    }

}
//B) Distribución por edad de demandantes
void calcularTotalesPorEdad(Registro *reg, int cant)
{
    Hogares hog[6][3] = {0};
    Registro *aux=reg;

    for(int i=0; i<cant;i++,aux++)
    {
        if(aux->TIPO_HOGAR_DCTOTAL == 1)
        {
            hog[aux->REGION-1][aux->TIPO_HOGAR_DCPOREDAD-1].sumWHOG+=aux->WHOG;
            hog[aux->REGION-1][aux->TIPO_HOGAR_DCPOREDAD-1].cantReg++;
        }
    }
    mostrarTotalesPorEdad(hog);
}
void mostrarTotalesPorEdad(const Hogares hog[6][3])
{
    printf("REGION\tTIPO_HOGAR_DCPOREDAD\tCantidad_Registro\tCantidad_Hogares_Dem_cuidado_Edad\n");
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            printf("%d\t%15d\t%20d\t%20d\n", i+1, j+1, hog[i][j].cantReg, hog[i][j].sumWHOG);
        }
    }

}
//C) Calculo de Proporciones
void calcularProporciones(Registro *reg, int cant)
{
    float prop[6][3] = {0};
    int sumTotalHog[6] = {0};

    Registro *aux=reg;
    Hogares hog[6][3] = {0};

    for(int i=0; i<cant;i++,aux++)
    {
        if(aux->TIPO_HOGAR_DCTOTAL == 1)
        {
            hog[aux->REGION-1][aux->TIPO_HOGAR_DCPOREDAD-1].sumWHOG+=aux->WHOG;
            hog[aux->REGION-1][aux->TIPO_HOGAR_DCPOREDAD-1].cantReg++;
        }

    }


    for(int i = 0; i < 6;i++)
    {

        for(int k=0;k<3;k++)
            sumTotalHog[i] += hog[i][k].sumWHOG;
        for(int j=0; j < 3; j++)
        {
            prop[i][j] = ((float)hog[i][j].sumWHOG/sumTotalHog[i])*100;
        }
    }

    mostrarProporciones(hog,prop,sumTotalHog);
    mostrarProporcionesRegion(prop);
}

void mostrarProporciones(Hogares h[6][3],float prop[6][3],int *suma)
{
    char *nomEdadDeman[] = {"hasta 13", "De 14 y mas", "Ambos Grupos Etarios"};

    printf("%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
        "REGION",
        "TIPO_HOGAR_DCPOREDAD"
        ,"Cantidad_Registro",
        "Cantidad_Hogares_Dem_cuidado_Edad",
        "Edad_Demandante_cuidado",
        "Hogares_Totales_DEM_cuidados",
        "Proporciones");
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            printf("%-20d%-20d%-20d%-20d%-20s%-20d%-20.2f\n",
                i+1,
                j+1,
                h[i][j].cantReg,
                h[i][j].sumWHOG,
                *(nomEdadDeman + j),
                *(suma + i),
                prop[i][j]);
        }
    }
}
void mostrarProporcionesRegion(float p[6][3])
{
    char *nomEdadDeman[] = {"hasta 13", "De 14 y mas", "Ambos Grupos Etarios"};

    printf("\n%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
        "Edad_dem_cuidado",
        "GBA",
        "PAMPEANA",
        "NOROESTE",
        "NORESTE",
        "CUYO",
        "PATAGONIA");

    for(int i = 0; i < 3; i++)
    {
        printf("%-20s",*(nomEdadDeman + i));
        for(int j = 0; j < 6; j++)
        {
            printf("%-20.2f",
                p[j][i]);
        }
        putchar('\n');
    }
}
//punto 5
void calcularDistribucionPorcentual(Registro *reg, int cant)
{
    float prop[6][2] = {0};
    int sumTotalHog[6] = {0};
    int hog[6][2] = {0};
    Registro *aux=reg;

    for(int i=0; i<cant;i++,aux++)
    {
        if(aux->TIPO_HOGAR_DCTOTAL == 1)
        {
            hog[aux->REGION-1][aux->CUIDADO_SOLO_HOGAR]+=aux->WHOG;
        }
    }


    for(int i = 0; i < 6;i++)
    {
        for(int k=0;k<2;k++)
            sumTotalHog[i] += hog[i][k];
        for(int j=0; j < 2; j++)
        {
            prop[i][j] = ((float)hog[i][j]/sumTotalHog[i])*100;
        }
    }
    mostrarDistribucionPorcentual(prop);
}

void mostrarDistribucionPorcentual(float prop[6][2])
{
    char *nomCuidadoHog[] = {"ninguno recibibe cuidado exclusivo del hogar", "cuidado exclusivo del propio hogar"};

    printf("\n%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
        "Cuidado_solo_hogar",
        "GBA",
        "PAMPEANA",
        "NOROESTE",
        "NORESTE",
        "CUYO",
        "PATAGONIA");

    for(int i = 0; i < 2; i++)
    {
        printf("%-50s",*(nomCuidadoHog + i));
        for(int j = 0; j < 6; j++)
        {
            printf("%-20.2f",
                prop[j][i]);
        }
        putchar('\n');
    }
}



//punto 8
void construirArchTiempo(Registro *reg,int cant)
{
    FILE *pt= fopen("REG_TIEMPO.dat","wb");
    tArchivoTiempo archT;
    char *tipo_trabajo[]={"Trabajo total","Ocupacion y autoconsumo","trabajo no remunerado (TNR)","Personales"};
    int tcs[4];
    int tp[4];

    if(pt == NULL)
    {
        printf("ERROR al abrir archivo.\n");
        exit(1);
    }
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<cant;i++)
        {
            archT.id = reg[i].ID;
            archT.whog = reg[i].WHOG;
            archT.wper = reg[i].WPER;
            archT.region = reg[i].REGION;
            archT.sexo = reg[i].SEXO_SEL;

            if(reg[i].EDAD_SEL >= 14 && reg[i].EDAD_SEL <= 29)
            strcpy(archT.grupoEdad,"14 a 29 anios");
            if(reg[i].EDAD_SEL >= 30 && reg[i].EDAD_SEL <= 64)
            strcpy(archT.grupoEdad,"30 a 64 anios");
            if(reg[i].EDAD_SEL >= 65)
            strcpy(archT.grupoEdad,"65 anios o mas");

            tp[0] = reg[i].TP_GRANGRUPO_TRABAJOTOTAL;
            tp[1] = reg[i].TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
            tp[2] = reg[i].TP_GRANGRUPO_TNR;
            tp[3] = reg[i].TP_GRANGRUPO_PERSONALES;

            tcs[0] = reg[i].TCS_GRANGRUPO_TRABAJOTOTAL;
            tcs[1] = reg[i].TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO;
            tcs[2] = reg[i].TCS_GRANGRUPO_TNR;
            tcs[3] = reg[i].TCS_GRANGRUPO_PERSONALES;

            strcpy(archT.tipoTrabajo, tipo_trabajo[j]);
            archT.tiempo = tcs[j];
            archT.valor = tp[j];
            fwrite(&archT,sizeof(tArchivoTiempo),1,pt);
        }
    }

    fclose(pt);

    mostrarArchivoTiempo(pt);
}

void mostrarArchivoTiempo(FILE *p)
{
    int i=0;
    p = fopen("REG_TIEMPO.dat","rb");
    if (p==NULL)
    {
        printf("ERROR, archivo no se pudo abrir");
        exit(1);
    }

    tArchivoTiempo archT;
    printf("%10s%10s%10s%10s%10s%20s%25s%10s%10s\n","ID","WHOG","WPER","REGION","SEXO_SEL","G_EDAD","T_TRABAJO","VALOR","TIEMPO");
    fread(&archT,sizeof(tArchivoTiempo),1,p);
    while(!feof(p) && i<10)
    {
        i++;
        //if(strcmp(archT.tipoTrabajo,"Ocupacion y autoconsumo")==0){}
        printf("%10d%10d%10d%10d%10d%20s%25s%10d%10d\n",archT.id,
            archT.whog,
            archT.wper,
            archT.region,
            archT.sexo,
            archT.grupoEdad,
            archT.tipoTrabajo,
            archT.valor,
            archT.tiempo
        );

        fread(&archT,sizeof(tArchivoTiempo),1,p);
    }
    fclose(p);
}

//punto 9
void calcularTiempoProm()
{
    FILE *p= fopen("REG_TIEMPO.dat","rb");
    tArchivoTiempo archT;
    float prom[4];
    size_t numerador[4]={0},denominador[4]={0};

    if(p == NULL)
    {
        printf("ERROR al abrir archivo.\n");
        exit(1);
    }

    fread(&archT,sizeof(tArchivoTiempo),1,p);
    while(!feof(p))
    {
        if(strcmp(archT.tipoTrabajo,"Trabajo total")==0 && archT.valor == 1)
        {
            denominador[3]+= archT.wper;
            numerador[3] +=  archT.tiempo*archT.wper;
        }
        if(strcmp(archT.tipoTrabajo,"Ocupacion y autoconsumo")==0 && archT.valor == 1)
        {
                denominador[1]+= archT.wper;
            numerador[1] +=  archT.tiempo*archT.wper;
        }
        if(strcmp(archT.tipoTrabajo,"trabajo no remunerado (TNR)")==0 && archT.valor == 1)
        {
            denominador[2]+= archT.wper;
            numerador[2] +=  archT.tiempo*archT.wper;
        }
        if(strcmp(archT.tipoTrabajo,"Personales")==0 && archT.valor == 1)
        {
            denominador[0]+= archT.wper;
            numerador[0] +=  archT.tiempo*archT.wper;
        }
        fread(&archT,sizeof(tArchivoTiempo),1,p);
    }


    for(int i = 0; i<4 ;i++)
    {
        if(numerador[i]==0)
            printf("ERROR, NUMERADOR ES 0, NO SE REALIZO PROMEDIO. %d",i+1);
        else
            prom[i] = (float)numerador[i]/denominador[i];
    }

    fclose(p);
    mostrarTiempoProm(prom,4);
}

void mostrarTiempoProm(float *prom,int ce)
{
    char *tipo_trabajo[]={"Personales","Ocupacion y autoconsumo","trabajo no remunerado (TNR)","Trabajo total"};
    int horas,minutos;

    printf("%20s%20s\n","TIPO_TRABAJO","TCS_PROM_TIPO_OCU");
    for(int i=0;i<ce;i++)
    {
        horas=*prom/60;
        minutos=(int)*prom%60;
        printf("%20s%20d:%d\n",tipo_trabajo[i],horas,minutos);
        prom++;
    }
}

//punto 10