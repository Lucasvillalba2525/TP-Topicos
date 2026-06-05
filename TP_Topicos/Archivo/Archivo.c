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

int procesarCSV(const char *arch,Cmp cmp,Registro **reg,int *cantReg,Vector*vecInd)
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

    int cont=0;

    secpalCrear(linea, &seclec);
    secpalLeer(&seclec, &pal);
    Indice elem;
    while(!secpalFin(&seclec))
    {
        mistrcpy(elem.campo,pal.vpal);
        elem.nroindice=cont;

        vectorInsertarAlFinal(vecInd,&elem);
        secpalLeer(&seclec, &pal);
        cont++;
    }

    int cap = 10;

    Registro *miReg = malloc(cap * sizeof(Registro));

    if(!miReg)
    {
        fclose(fp);
        return ERROR_SIN_MEM;
    }

    Vector miVec;
    vectorCrear(&miVec,sizeof(int));

    *cantReg = 0;

    while(fgets(linea, MAX_LINEA, fp))
    {
        parseolinea(linea, &miVec, cmp);

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

        procesarDatos(&miVec,miReg,*cantReg,vecInd,cmp);

        (*cantReg)++;

        vectorEliminar(&miVec);
    }

    vectorDestruir(&miVec);
    fclose(fp);

    *reg = miReg;

    return TODO_OK;
}

void mistrcpy(char*dest,const char*orig)
{
    const char*i=orig;
    char*j=dest;

    while(*i!='\0')
    {
        *j=*i;
        i++;
        j++;
    }
    *j='\0';

}

void parseolinea(char *linea, Vector *vector, Cmp cmp)
{
    Secpal seclec;
    Palabra pal;

    secpalCrear(linea, &seclec);
    secpalLeer(&seclec, &pal);

    while(!secpalFin(&seclec))
    {
        int dato;

        if(cmpfaltante(&pal, "NA")==ENCONTRADO){
            dato = DATO_FALT;
        }
        else{
            dato = atoi(pal.vpal);
        }

        if (vectorInsertarAlFinal(vector,&dato) != TODO_OK)
        {
            printf("ERROR INSERT VECTOR\n");
            return;
        }

        secpalLeer(&seclec, &pal);
    }
}

void procesarDatos(Vector *vector, Registro *reg,int nroReg, Vector*vecInd,Cmp cmp)
{
    int iID,iWHOG,iWPER,iREGION,iEDAD,iSEXO,iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO,iTP_GRANGRUPO_TRABAJOTOTAL,
    iTP_GRANGRUPO_TNR,iTIPO_HOGAR_DCPOREDAD,iTIPO_HOGAR_DCTOTAL,iCUIDADO_SOLO_HOGAR,iTP_GRANGRUPO_PERSONALES,
    iTCS_GRANGRUPO_TRABAJOTOTAL,iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO,iTCS_GRANGRUPO_TNR,iTCS_GRANGRUPO_PERSONALES;

    int*v=vector->vec;

    Indice*ind=(Indice*)vecInd->vec;
    Registro*p=(Registro*)reg;

    for(int i=0;i<vecInd->ce;i++,ind++)
    {
        if(cmp("ID",ind->campo)==ENCONTRADO)iID=ind->nroindice;
        if(cmp("WHOG",ind->campo)==ENCONTRADO)iWHOG=ind->nroindice;
        if(cmp("WPER",ind->campo)==ENCONTRADO)iWPER=ind->nroindice;
        if(cmp("REGION",ind->campo)==ENCONTRADO)iREGION=ind->nroindice;
        if(cmp("EDAD_SEL",ind->campo)==ENCONTRADO)iEDAD=ind->nroindice;
        if(cmp("SEXO_SEL",ind->campo)==ENCONTRADO)iSEXO=ind->nroindice;
        if(cmp("TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO",ind->campo)==ENCONTRADO)iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO=ind->nroindice;
        if(cmp("TP_GRANGRUPO_TRABAJOTOTAL",ind->campo)==ENCONTRADO)iTP_GRANGRUPO_TRABAJOTOTAL=ind->nroindice;
        if(cmp("TP_GRANGRUPO_TNR",ind->campo)==ENCONTRADO)iTP_GRANGRUPO_TNR=ind->nroindice;
        if(cmp("TIPO_HOGAR_DCPOREDAD",ind->campo)==ENCONTRADO)iTIPO_HOGAR_DCPOREDAD=ind->nroindice;
        if(cmp("TIPO_HOGAR_DCTOTAL",ind->campo)==ENCONTRADO)iTIPO_HOGAR_DCTOTAL=ind->nroindice;
        if(cmp("CUIDADO_SOLO_HOGAR",ind->campo)==ENCONTRADO)iCUIDADO_SOLO_HOGAR=ind->nroindice;
        if(cmp("TP_GRANGRUPO_PERSONALES",ind->campo)==ENCONTRADO)iTP_GRANGRUPO_PERSONALES=ind->nroindice;
        if(cmp("TCS_GRANGRUPO_TRABAJOTOTAL",ind->campo)==ENCONTRADO)iTCS_GRANGRUPO_TRABAJOTOTAL=ind->nroindice;
        if(cmp("TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO",ind->campo)==ENCONTRADO)iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO=ind->nroindice;
        if(cmp("TCS_GRANGRUPO_TNR",ind->campo)==ENCONTRADO)iTCS_GRANGRUPO_TNR=ind->nroindice;
        if(cmp("TCS_GRANGRUPO_PERSONALES",ind->campo)==ENCONTRADO)iTCS_GRANGRUPO_PERSONALES=ind->nroindice;
    }

    (p+nroReg)->ID     = *(v+iID);

    (p+nroReg)->WHOG     = *(v+iWHOG);

    (p+nroReg)->WPER     = *(v+iWPER);

    (p+nroReg)->REGION   = *(v+iREGION);

    (p+nroReg)->EDAD_SEL = *(v+iEDAD);

    (p+nroReg)->SEXO_SEL = *(v+iSEXO);

    (p+nroReg)->TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO   = *(v+iTP_GRANGRUPO_OCUPACIONYAUTOCONSUMO);

    (p+nroReg)->TP_GRANGRUPO_TRABAJOTOTAL = *(v+iTP_GRANGRUPO_TRABAJOTOTAL);

    (p+nroReg)->TP_GRANGRUPO_TNR = *(v+iTP_GRANGRUPO_TNR);

    (p+nroReg)->TIPO_HOGAR_DCPOREDAD = *(v+iTIPO_HOGAR_DCPOREDAD);

    (p+nroReg)->TIPO_HOGAR_DCTOTAL = *(v+iTIPO_HOGAR_DCTOTAL);

    (p+nroReg)->CUIDADO_SOLO_HOGAR = *(v+iCUIDADO_SOLO_HOGAR);

    (p+nroReg)->TP_GRANGRUPO_PERSONALES = *(v+iTP_GRANGRUPO_PERSONALES);

    (p+nroReg)->TCS_GRANGRUPO_TRABAJOTOTAL = *(v+iTCS_GRANGRUPO_TRABAJOTOTAL);

    (p+nroReg)->TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO = *(v+iTCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO);

    (p+nroReg)->TCS_GRANGRUPO_TNR = *(v+iTCS_GRANGRUPO_TNR);

    (p+nroReg)->TCS_GRANGRUPO_PERSONALES = *(v+iTCS_GRANGRUPO_PERSONALES);

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

//punto 2
void clasificacionRangoEtario(Registro *reg,int cantReg)
{
    Registro*p=reg;

    for(int i=0;i<cantReg;i++,p++)
    {
        if(p->EDAD_SEL>=14 && p->EDAD_SEL<=29)
            mistrcpy(p->GRUPO_EDAD_SEL,"14 a 29 anios");
        if(p->EDAD_SEL>=30 && p->EDAD_SEL<=64)
            mistrcpy(p->GRUPO_EDAD_SEL,"30 a 64 anios");
        if(p->EDAD_SEL>=65)
            mistrcpy(p->GRUPO_EDAD_SEL,"65 anios y mas");
    }

    mostrarRangoEtarios(reg,cantReg);
}

void mostrarRangoEtarios(Registro *reg,int cant)
{
    printf("%10s%10s%10s%10s%10s%10s%20s%10s%10s%20s\n","ID","WHOG","WPER","REGION","SEXO_SEL","EDAD_SEL","OCUPACIONYAUTO","TRAB.TOTAL","TNR","GRUPO_EDAD_SEL");


    for(int i = 0; i < cant; i++)
    {
        printf("%10d\t%10d\t%10d\t%10d\t%10d\t%10d\t%10d\t%10d\t%10d\t%20s\n",
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

    printf("\n%50s%10s%10s%10s%10s%10s%10s\n",
        "Cuidado_solo_hogar",
        "GBA",
        "PAMPEANA",
        "NOROESTE",
        "NORESTE",
        "CUYO",
        "PATAGONIA");

    for(int i = 0; i < 2; i++)
    {
        printf("%50s",*(nomCuidadoHog + i));
        for(int j = 0; j < 6; j++)
        {
            printf("%10.2f",
                prop[j][i]);
        }
        putchar('\n');
    }
}

//punto 6

void** crearMatriz(int filas, int columnas, size_t tamElem)
{
    void** mat = malloc(filas * sizeof(void*));

    if(!mat)
    {
        return NULL;
    }

    void** ult = mat + (filas - 1);

    for(void** i = mat; i <= ult; i++)
    {
        *i = calloc(columnas , tamElem);

        if(!*i)
        {
            destruirMatriz(mat, i - mat);
            return NULL;
        }
    }

    return mat;
}

void destruirMatriz(void** mat, int filas)
{
    void** ult = mat + (filas - 1);

    for(void** i = mat; i <= ult; i++)
    {
        free(*i);
    }

    free(mat);
}

size_t Sumadeponderadores(Registro *reg,int cantReg,int **mat,int columnas,ObtenerFila obtenerFila,Acum acum,int*vecAcum)
{
    Registro *p = reg;
    int fila;
    size_t cantTotPer=0;

    for (int i = 0; i < cantReg; i++,p++)
    {
        fila = obtenerFila(p);
        vecAcum[fila]+=p->WPER;

        if (p->TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO == 1)
            acum(mat, fila, 0, p->WPER);


        if (p->TP_GRANGRUPO_TNR == 1)
            acum(mat, fila, 1, p->WPER);


        if (p->TP_GRANGRUPO_TRABAJOTOTAL == 1)
            acum(mat, fila, 2, p->WPER);


        cantTotPer+=p->WPER;
    }

    return cantTotPer;
}

int calcularPorcentaje(int** mat,int fil,int col,float**matPorc,int CantPer,int* vecAcum,char*Filas[])
{
    for(int i=0;i<fil-1;i++)
    {
        for(int j=0;j<col;j++)
            matPorc[i][j]=(float)mat[i][j]/vecAcum[i]*100;

    }

    for(int k=0;k<col;k++)
        matPorc[fil-1][k]=(float)mat[fil-1][k]/CantPer*100;

    //mostrarMatrizPorc(matPorc,fil,col,Filas); ESTA ES LA FUNCION DONDE MUESTRA LAS MATRICES EN EL .C, TAMBIEN ESTA LA OTRA EN EL .BIN

    return TODO_OK;
}

void mostrarMatrizPorc(float**matPorc,int fil,int col,char*Filas[])
{
    printf("%-20s", *Filas);
    printf("%-20s%-20s%-20s\n", "OcupacYAut","TNR","TrabajTot");

    for(int i=0;i<fil;i++)
    {
        printf("%-20s", *(Filas + i + 1));
        for(int j=0;j<col;j++)
            printf("%-18.2f\t",matPorc[i][j]);

        putchar('\n');
    }

}

int guardarMatrizBin(const char*Arch,float**mat,int fil,int col)
{
    FILE*pf=fopen(Arch,"wb");
    if(!pf)
        return ERROR_ARCHIVO;

    fwrite(&fil,sizeof(int),1,pf);
    fwrite(&col,sizeof(int),1,pf);

    for(int i=0;i<fil;i++)
        fwrite(mat[i],sizeof(float),col,pf);

    fclose(pf);

    return TODO_OK;
}

int leerMatrizBin(const char *Arch, int fil, int col,char*Filas[])
{
    FILE *fp = fopen(Arch, "rb");
    if(!fp)
        return ERROR_ARCHIVO;

    fread(&fil, sizeof(int), 1, fp);
    fread(&col, sizeof(int), 1, fp);

    float **mat = (float**)crearMatriz(fil, col, sizeof(float));
    if(!mat)
        return ERROR_SIN_MEM;

    for(int i = 0; i < fil; i++)
        fread(mat[i], sizeof(float),col,fp);

    mostrarMatrizPorc(mat,fil,col,Filas);

    fclose(fp);
    return TODO_OK;
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
    p = fopen("REG_TIEMPO.dat","rb");
    if (p==NULL)
    {
        printf("ERROR, archivo no se pudo abrir");
        exit(1);
    }

    tArchivoTiempo archT;
    printf("%10s%10s%10s%10s%10s%20s%25s%10s%10s\n","ID","WHOG","WPER","REGION","SEXO_SEL","G_EDAD","T_TRABAJO","VALOR","TIEMPO");
    fread(&archT,sizeof(tArchivoTiempo),1,p);
    while(!feof(p))
    {

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
            else{

                prom[i] = (float)numerador[i]/denominador[i];
            }
    }

    fclose(p);
    mostrarTiempoProm(prom,4);
}


void mostrarTiempoProm(float *prom,int ce)
{
    char *tipo_trabajo[]={"Personales","Ocupacion y autoconsumo","trabajo no remunerado (TNR)","Trabajo total"};
    int horas,minutos;

    printf("%40s%20s\n","TIPO_TRABAJO","TCS_PROM_TIPO_OCU");
    for(int i=0;i<ce;i++)
    {
        horas=*prom/60;
        minutos=(int)*prom%60;
        printf("%40s%20d:%d\n",tipo_trabajo[i],horas,minutos);
        prom++;
    }
}

//punto 10
void calcularTiempoPromSexo()
{
    FILE *p= fopen("REG_TIEMPO.dat","rb");
    tArchivoTiempo archT;
    float promH[4];
    float promM[4];
    size_t numerador[2][4]={0},denominador[2][4]={0};

    if(p == NULL)
    {
        printf("ERROR al abrir archivo.\n");
        exit(1);
    }

    fread(&archT,sizeof(tArchivoTiempo),1,p);
    while(!feof(p))
    {
         if(strcmp(archT.tipoTrabajo,"Personales")==0 && archT.valor == 1)
        {
            if(archT.sexo==1){
                denominador[archT.sexo-1][0]+= archT.wper;
                numerador[archT.sexo-1][0] +=  archT.tiempo*archT.wper;
            }else{
                denominador[archT.sexo-1][0]+= archT.wper;
                numerador[archT.sexo-1][0] +=  archT.tiempo*archT.wper;
            }
        }
        if(strcmp(archT.tipoTrabajo,"Ocupacion y autoconsumo")==0 && archT.valor == 1)
        {
            if(archT.sexo==1){
                denominador[archT.sexo-1][1]+= archT.wper;
                numerador[archT.sexo-1][1] +=  archT.tiempo*archT.wper;
            }else{
                denominador[archT.sexo-1][1]+= archT.wper;
                numerador[archT.sexo-1][1] +=  archT.tiempo*archT.wper;
            }
        }
        if(strcmp(archT.tipoTrabajo,"trabajo no remunerado (TNR)")==0 && archT.valor == 1)
        {
            if(archT.sexo==1){
                denominador[archT.sexo-1][2]+= archT.wper;
                numerador[archT.sexo-1][2] +=  archT.tiempo*archT.wper;
            }else{
                denominador[archT.sexo-1][2]+= archT.wper;
                numerador[archT.sexo-1][2] +=  archT.tiempo*archT.wper;
            }
        }
        if(strcmp(archT.tipoTrabajo,"Trabajo total")==0 && archT.valor == 1)
        {
            if(archT.sexo==1){
                denominador[archT.sexo-1][3]+= archT.wper;
                numerador[archT.sexo-1][3] +=  archT.tiempo*archT.wper;
            }else{
                denominador[archT.sexo-1][3]+= archT.wper;
                numerador[archT.sexo-1][3] +=  archT.tiempo*archT.wper;
            }
        }

        fread(&archT,sizeof(tArchivoTiempo),1,p);
    }


    for(int i = 0; i<2 ;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(numerador[i][j]==0)
                printf("ERROR, NUMERADOR ES 0, NO SE REALIZO PROMEDIO. %d",i+1);
            else{
                if(i)
                {
                    promH[j] = (float)numerador[i][j]/denominador[i][j];
                }else
                {
                    promM[j] = (float)numerador[i][j]/denominador[i][j];
                }
            }
        }

    }

    fclose(p);
    mostrarTiempoPromSexo(promH,promM,4,2);
}

void mostrarTiempoPromSexo(float *promH,float *promM,int c,int f)
{
    char *tipo_sexo[]={"MUJERES","VARONES"};
    int horas,minutos;

    printf("%20s%20s%30s%30s%20s","SEXO_SEL","Personales","Ocupacion y autoconsumo","trabajo no remunerado (TNR)","Trabajo total");
    for(int i=0;i<f;i++)
    {
        printf("\n%20s",tipo_sexo[i]);
        for(int j=0;j<c;j++)
        {
            if(i)
            {
                horas=promH[j]/60;
                minutos=(int)promH[j]%60;
                //promM--;
            }else{
                horas=promM[j]/60;
                minutos=(int)promM[j]%60;
                //promH--;
            }
            printf("%20d:%02d",horas,minutos);
        }
    }
}

//punto 11
void calcularTiempoPromEdad()
{
    FILE *p= fopen("REG_TIEMPO.dat","rb");
    tArchivoTiempo archT;
    float prom[3][4];
    size_t numerador[3][4]={0},denominador[3][4]={0};
    int pos;

    if(p == NULL)
    {
        printf("ERROR al abrir archivo.\n");
        exit(1);
    }

    fread(&archT,sizeof(tArchivoTiempo),1,p);
    while(!feof(p))
    {
         if(strcmp(archT.tipoTrabajo,"Personales")==0 && archT.valor == 1)
        {
            pos = cmpRangoEdad(archT.grupoEdad);
            denominador[pos][0]+= archT.wper;
            numerador[pos][0] +=  archT.tiempo*archT.wper;
        }
        if(strcmp(archT.tipoTrabajo,"Ocupacion y autoconsumo")==0 && archT.valor == 1)
        {
            pos = cmpRangoEdad(archT.grupoEdad);
            denominador[pos][1]+= archT.wper;
            numerador[pos][1] +=  archT.tiempo*archT.wper;

        }
        if(strcmp(archT.tipoTrabajo,"trabajo no remunerado (TNR)")==0 && archT.valor == 1)
        {
            pos = cmpRangoEdad(archT.grupoEdad);
            denominador[pos][2]+= archT.wper;
            numerador[pos][2] +=  archT.tiempo*archT.wper;
        }
        if(strcmp(archT.tipoTrabajo,"Trabajo total")==0 && archT.valor == 1)
        {
            pos = cmpRangoEdad(archT.grupoEdad);
            denominador[pos][3]+= archT.wper;
            numerador[pos][3] +=  archT.tiempo*archT.wper;
        }

        fread(&archT,sizeof(tArchivoTiempo),1,p);
    }


    for(int i = 0; i<3 ;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(numerador[i][j]==0)
                printf("ERROR, NUMERADOR ES 0, NO SE REALIZO PROMEDIO. %d",i+1);
            else{
                prom[i][j] = (float)numerador[i][j]/denominador[i][j];
            }
        }

    }

    fclose(p);
    mostrarTiempoPromEdad(prom,4,3);
}

int cmpRangoEdad(char *cmp){
    char *nomRangoEdad[]={"14 a 29 anios","30 a 64 anios","65 anios o mas"};
    int pos=-1;

    if(strcmp(cmp,nomRangoEdad[0])==0){
        pos = 0;
    }else{
        if(strcmp(cmp,nomRangoEdad[1])==0)
        {
            pos=1;
        }else{
            pos=2;
        }
    }

    return pos;
}

void mostrarTiempoPromEdad(float prom[3][4],int c,int f)
{
    char *tipo_gEdad[]={"14 a 29 anios","30 a 64 anios","65 anios o mas"};
    int horas,minutos;

    printf("%20s%20s%30s%30s%20s","GRUPO_EDAD","Personales","Ocupacion y autoconsumo","trabajo no remunerado (TNR)","Trabajo total");
    for(int i=0;i<f;i++)
    {
        printf("\n%20s",tipo_gEdad[i]);
        for(int j=0;j<c;j++)
        {
            horas=prom[i][j]/60;
            minutos=(int)prom[i][j]%60;
            printf("%20d:%02d",horas,minutos);
        }
    }
}
