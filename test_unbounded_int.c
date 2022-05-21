#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include "unbounded_int.h"

void afficher(unbounded_int u){//Maintenant qu'on a la conversion en string on pourrait juste convertir en string et afficher
    if(u.signe=='*'){
        printf("echec, le signe est : '*'");
        return;
    }
    printf("%c",u.signe);
    chiffre *current = u.premier;
    while(current != NULL){
        printf("%c",current->c);
        current = current->suivant;
    }
}

void afficherBis(unbounded_int u){
    afficher(u);
    printf("\n");
}

int main(void) {
    unbounded_int u = string2unbounded_int("-125");
    afficherBis(u);
    printf("%c\n", u.signe);
    unbounded_int ui = string2unbounded_int("az-125");
    afficherBis(ui);
    long long i = 102568;
    unbounded_int ul = ll2unbounded_int(i);
    afficherBis(ul);
    printf("%s",unbounded_int2string(u));
    free_unbounded_int(u);
    free_unbounded_int(ui);
    free_unbounded_int(ul);
    
    return EXIT_SUCCESS;
}