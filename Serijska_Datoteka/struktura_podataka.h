#ifndef STRUKTURA_PODATAKA_H_INCLUDED
#define STRUKTURA_PODATAKA_H_INCLUDED

#define FAKTOR_BLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE "********"

typedef enum TipDogadjaja{
    INFO,
    WARNING,
    ERROR
}TIP_DOGADJAJA;

typedef struct Slog{
    char evidBroj[12+1];
    char vremeDogadjaja[19+1];
    TIP_DOGADJAJA tip;
    char korisnickiId[10+1];
    char nazivDogadjaja[20+1];
    int deleted;
}SLOG;

typedef struct Blok{
    SLOG slogovi[FAKTOR_BLOKIRANJA];
}BLOK;

#endif // STRUKTURA_PODATAKA_H_INCLUDED
