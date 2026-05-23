#include"C:\Users\Argentech\Desktop\TP_Topicos\Archivo\Archivo.h"
#define CSV 2
#define BIN 3
#define TIPOTXT 1
#define ARG_CSV 1

int procesarCSV(const char *file);

int main(int argc,char* argv[])
{
    procesarCSV(argv[ARG_CSV]);
    return 0;
}

int procesarCSV(const char *arch)
{
    FILE *fp = fopen(arch, "r");
    if (!fp)
    {
        printf("Error archivo\n");
        return ERROR_ARCHIVO;
    }

    char linea[MAX_LINEA];

    if(!fgets(linea, sizeof(linea), fp))//Lee encabezado
    {
        fclose(fp);
        return FALLA_LEC;
    }

    Secpal seclec;
    Palabra pal;
    int iID,iWHOG,iWPER,iREGION,iSEXO,iEDAD,cont=0;
    //int ivec[CANT_CAMPOS-1];
    secpalCrear(linea,&seclec);
    secpalLeer(&seclec,&pal);
    while(!secpalFin(&seclec))
    {
        if(buscar(&pal, "ID")==true)
            iID=cont;

        if(buscar(&pal, "WHOG")==true)
            iWHOG=cont;

        if(buscar(&pal, "WPER")==true)
            iWPER=cont;

        if(buscar(&pal, "REGION")==true)
            iREGION=cont;

        if(buscar(&pal, "SEXO_SEL")==true)
            iSEXO=cont;

        if(buscar(&pal, "EDAD_SEL")==true)
            iEDAD=cont;

        secpalLeer(&seclec,&pal);
        cont++;
    }

    RegIndice reg;
    reg.iID=iID;
    reg.iWHOG=iWHOG;
    reg.iWPER=iWPER;
    reg.iREGION=iREGION;
    reg.iSEXO_SEL=iSEXO;
    reg.iEDAD_SEL=iEDAD;

    printf("ID:%d\nWHOG:%d\nWPER:%d\nREGION:%d\nSEXO:%d\nEDAD:%d",reg.iID,reg.iWHOG,reg.iWPER,reg.iREGION,reg.iSEXO_SEL,reg.iEDAD_SEL);






    fclose(fp);

    return TODO_OK;

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



