#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H
#include<stdlib.h>

typedef struct chiffre{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct{
    char signe; /* soit ’+’ soit ’-’ */
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
} unbounded_int;

void free_unbounded_int(unbounded_int u);

unbounded_int string2unbounded_int(const char *e);

unbounded_int ll2unbounded_int(long long i);

#endif