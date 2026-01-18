#include <stdio.h>
#include <stdlib.h>

#include "operacije.h"

int main()
{
    int running = 1;
    int userInput;

    FILE *fajl = NULL;

    char txt[20];
    printf("Unesite naziv tekstualne datoteke: ");
    scanf("%s", txt);
    getc(stdin);

    //podaci ucitani u bin
    citajTxtOtvoriBin(txt);
    printf("\n");

    while(running){
        printf("\nIzaberite opciju: ");
        printf("\n1 - Otvaranje datoteke");
        printf("\n2 - Formiranje datoteke");
        printf("\n3 - Unos novog sloga");
        printf("\n4 - Ispis svih slogova");
        printf("\n5 - Pretraga datoteke");
        printf("\n6 - Modifikacija sloga");
        printf("\n7 - Logicko brisanje sloga");
        printf("\n0 - Izlaz");
        printf("\n");

        scanf(" %d", &userInput);
        printf("DEBUG: userInput = [%d]\n", userInput);

        getc(stdin);

        switch(userInput)
        {
            case 1:
            {
                char nazivDatoteke[20];
                printf("Unesite ime datoteke za otvaranje: ");
                scanf("%s", &nazivDatoteke[0]);
                fajl = otvoriDatoteku(nazivDatoteke);
                printf("\n");
                break;
            }
            case 2:
            {
                char nazivDatoteke[20];
                printf("Unesite ime datoteke za kreiranje: ");
                scanf("%s", nazivDatoteke);
                kreirajDatoteku(nazivDatoteke);
                fajl = otvoriDatoteku(nazivDatoteke);
                printf("\n");
                break;
            }
            case 3:
            {
                if(fajl == NULL){
                    printf("\nNeuspesno otvoren fajl");
                    break;
                }
                SLOG temp;
                char tipString[15];


                printf("Unesite evidencioni broj, vreme dogadaja, tip dogadjaja, korisnicki ID, naziv dogadjaja: ");
                scanf("%s %s %s %s %s", temp.evidBroj, temp.vremeDogadjaja, tipString, temp.korisnickiId, temp.nazivDogadjaja);
                if(strcmp(tipString, "INFO") == 0) temp.tip = INFO;
                else if(strcmp(tipString, "WARNING") == 0) temp.tip = WARNING;
                else if(strcmp(tipString, "ERROR") == 0) temp.tip = ERROR;
                else temp.tip = INFO;

                temp.deleted = 0;
                dodajSlog(fajl, &temp);
                break;
            }
            case 4:
            {
                ispisiSveSlogove(fajl);
                break;
            }
            case 5:
            {
                char evidBroj[12+1];
                printf("\nUnesite trazeni evidencioni broj: ");
                scanf("%s", &evidBroj);
                SLOG *trazeni = pronadjiSlog(fajl, evidBroj);
                if(trazeni == NULL){
                    printf("Nije pronadjen slog.");
                }else{
                    printf("%-12s %-19s %-7s %-10s %-20s", "EVB", "VREME", "TIP", "KID", "NAZ");
                    ispisiSlog(trazeni);
                    printf("\n");
                }
                break;
            }
            case 6:
            {
                char evidBroj[12+1];
                printf("\nUnesite evid. broj sloga koji azurirate: ");
                scanf("%s", evidBroj);
                char nazivDogadjaja[20+1];
                printf("\nUnesite novi naziv dogadjaja: ");
                scanf("%s", nazivDogadjaja);
                modifikujSlog(fajl, evidBroj, nazivDogadjaja);
                printf("\n");
                break;
            }
            case 7:
            {
                char evidBroj[12+1];
                printf("Unesite evidencioni broj sloga koji zelite logicki da obrisete: ");
                scanf("%s", evidBroj);
                logickoBrisanjeSloga(fajl, evidBroj);
                printf("\n");
                break;
            }
            case 0:
            {
                if(fajl != NULL){
                    fclose(fajl);
                }
                running = 0;
            }
        }
    }

    return 0;
}
