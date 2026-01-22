#include "operacije.h"

void citajTxtOtvoriBin(char *txtNaziv){
    FILE *txt = fopen(txtNaziv, "r+");
    if(txt == NULL){
        printf("\nGreska pri otvaranju tekstualne datoteke.");
        return;
    }else{
        printf("\nTekstualna datoteka uspesno otvorena!");
    }
    //sada kreiramo praznu binarnu datoteku
    kreirajDatoteku("podaci.dat");
    FILE *bin = otvoriDatoteku("podaci.dat");
    if(bin == NULL){
        printf("\nGreska prilikom otvaranja binarne datoteke!");
        return;
    }
    printf("\nBinarna datoteka uspesno otvorena!");

    SLOG *temp = (SLOG*)malloc(sizeof(SLOG));
    char tipString[15];
    fseek(txt, 0, SEEK_SET);
    while(fscanf(txt, "%12s %19s %7s %10s %20s %d",
                 temp->evidBroj,
                 temp->vremeDogadjaja,
                 tipString,
                 temp->korisnickiId,
                 temp->nazivDogadjaja,
                 &temp->deleted) == 6){
            if(strcmp(tipString, "INFO") == 0) temp->tip = INFO;
            else if(strcmp(tipString, "WARNING") == 0) temp->tip = WARNING;
            else if(strcmp(tipString, "ERROR") == 0) temp->tip = ERROR;
            else temp->tip = INFO;
            dodajSlog(bin, temp);
            }

        fflush(bin);
        free(temp);
        fclose(txt);
        fclose(bin);
}

FILE *otvoriDatoteku(char *nazivDatoteke){
    FILE *fajl = fopen(nazivDatoteke, "rb+");
    if(fajl == NULL){
        printf("\nFajl nije ispravno otvoren, pokusajte ponovo.");
    }else{
        printf("\nFajl uspesno otvoren!");
    }
    return fajl;
}

void kreirajDatoteku(char *nazivDatoteke){
    FILE *fajl = fopen(nazivDatoteke, "wb");
    if(fajl == NULL){
        printf("\nFajl neuspesno kreiran. Pokusajte ponovo.");
    }else{
        BLOK blok;
        strcpy(blok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        printf("\nDatoteka uspesno kreirana!");
        fclose(fajl);
    }
}

SLOG *pronadjiSlog(FILE *fajl, char *evidBroj){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja fajla. Pokusajte ponovo.");
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    //dosli smo do kraja datoteke i nismo pronasli slog = ne postoji
                    return NULL;
                }
                if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted){
                    //pronasli smo slog sa trazenim evidencionim brojem i nije logicki obrisan
                    SLOG *slog = (SLOG*)malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                    return slog;
                }
            }
        }
        return NULL;
    }
}

void dodajSlog(FILE *fajl, SLOG *slog){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja datoteke. Pokusajte ponovo.");
        return;
    }else{
        SLOG *stariSlog = pronadjiSlog(fajl, slog->evidBroj);
        if(stariSlog != NULL){
            printf("\nVec postoji slog sa ovim evidencionim brojem. Pokusajte ponovo.");
            return;
        }
        BLOK blok;
        fseek(fajl, -sizeof(BLOK), SEEK_END);
        fread(&blok, sizeof(BLOK), 1, fajl);

        int i;
        for(i = 0; i < FAKTOR_BLOKIRANJA; i++){
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
                break;
            }
        }
        i++;
        if(i < FAKTOR_BLOKIRANJA){
            strcpy(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE);
            fseek(fajl, -sizeof(BLOK), SEEK_CUR);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            fflush(fajl);
        }else{
            fseek(fajl, -sizeof(BLOK), SEEK_CUR);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
            BLOK noviBlok;
            strcpy(noviBlok.slogovi[0].evidBroj, OZNAKA_KRAJA_DATOTEKE);
            fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
        }
    }
}

void ispisiSlog(SLOG *slog){
    char tipString[15];
    printf("%12s %19s %7s %10s %20s",
           slog->evidBroj,
           slog->vremeDogadjaja,
           tipString,
           slog->korisnickiId,
           slog->nazivDogadjaja);
    if(strcmp(tipString, "INFO") == 0) slog->tip = INFO;
    else if(strcmp(tipString, "WARNING") == 0) slog->tip = WARNING;
    else if(strcmp(tipString, "ERROR") == 0) slog->tip = ERROR;
}

void ispisiSveSlogove(FILE *fajl){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja fajla. Probajte ponovo.");
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        int rbBloka = 0;
        printf("BL: SL: Evidencioni broj:  Vreme dogadjaja:  Tip dogadjaja:  Korisnicki ID:  Naziv dogadjaja: \n");
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    printf("B%d S%d *\n", rbBloka, i);
                    break;
                }
                if(!blok.slogovi[i].deleted){
                    printf("B%d S%d", rbBloka, i);
                    ispisiSlog(&blok.slogovi[i]);
                    break;
                }
            }
            rbBloka++;
        }
    }
}

void modifikujSlog(FILE *fajl, char *evidBroj, char *naziv){
    if(fajl == NULL){
        printf("\nGreska pri otvaranju fajla.");
        return;
    }

    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    while(fread(&blok, sizeof(BLOK), 1, fajl)){
        for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
            if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE)== 0){
                printf("\nSlog ne postoji.");
                break;
            }
            if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted){
                strcpy(blok.slogovi[i].nazivDogadjaja, naziv);
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                printf("\nSlog uspesno izmenjen.");
                return;
            }
        }
    }
}

void logickoBrisanjeSloga(FILE *fajl, char *evidBroj){
    if(fajl == NULL){
        printf("\nGreska prilikom otvaranja datoteke.");
        return;
    }else{
        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        while(fread(&blok, sizeof(BLOK), 1, fajl)){
            for(int i = 0; i < FAKTOR_BLOKIRANJA; i++){
                if(strcmp(blok.slogovi[i].evidBroj, OZNAKA_KRAJA_DATOTEKE) == 0){
                    printf("Nismo naisli na trazeni slog.");
                    break;
                }
                if(strcmp(blok.slogovi[i].evidBroj, evidBroj) == 0 && !blok.slogovi[i].deleted){
                    blok.slogovi[i].deleted = 1;
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);

                    printf("\nSLOG JE LOGICKI OBRISAN.");
                    return;
                }
            }
        }
    }
}

