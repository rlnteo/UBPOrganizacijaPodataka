#ifndef OPERACIJE_H_INCLUDED
#define OPERACIJE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struktura_podataka.h"

void citajTxtOtvoriBin(char *txtNaziv);

FILE *otvoriDatoteku(char *nazivDatoteke);
void kreirajDatoteku(char *nazivDatoteke);
SLOG *pronadjiSlog(FILE *fajl, char *evidBroj);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSlog(SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void modifikujSlog(FILE *fajl, char *evidBroj, char *naziv);
void logickoBrisanjeSloga(FILE *fajl, char *evidBroj);
void fizickoBrisanjeSloga(FILE *fajl, char *evidBroj);

#endif // OPERACIJE_H_INCLUDED
