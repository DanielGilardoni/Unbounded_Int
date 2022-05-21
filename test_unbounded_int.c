#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include "unbounded_int.h"

void afficher(unbounded_int u){//Maintenant qu'on a la conversion en string on pourrait juste convertir en string et afficher
    if(u.signe !='+' && u.signe != '-'){
        printf("echec, le signe est incorrecte");
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
    //test de string -> unbounded_int :
    printf("\ntest de string -> unbounded_int :\n");
    unbounded_int u = string2unbounded_int("-125");
    afficherBis(u);
    unbounded_int ui = string2unbounded_int("az-125");
    afficherBis(ui);

    //test de long long -> unbounded_int :
    printf("\ntest de long long -> unbounded_int :\n");
    long long i = 9876543210;
    unbounded_int ul = ll2unbounded_int(i);
    afficherBis(ul);

    //test de unbounded_int -> string :
    printf("\ntest de unbounded_int -> string :\n");
    printf("%s\n",unbounded_int2string(u));
    printf("%s\n",unbounded_int2string(ui));
    printf("%s\n",unbounded_int2string(ul));

    //test de comparaison entre 2 unbounded_int :
    printf("\ntest de comparaison entre 2 unbounded_int :\n");
    int res;
    res = unbounded_int_cmp_unbounded_int(u,ul);
    printf("%d\n", res);
    res = unbounded_int_cmp_unbounded_int(ul,u);
    printf("%d\n", res);
    res = unbounded_int_cmp_unbounded_int(u,u);
    printf("%d\n", res);

    //test de comparaison entre un unbounded_int et un long long :
    printf("\ntest de comparaison entre un unbounded_int et un long long :\n");
    res = unbounded_int_cmp_ll(u, 14789653214587);
    printf("%d\n", res);
    res = unbounded_int_cmp_ll(ul, 987654321);
    printf("%d\n", res);
    res = unbounded_int_cmp_ll(ul, 9876543210);
    printf("%d\n", res);
    res = unbounded_int_cmp_ll(ul, -9876543210);
    printf("%d\n", res);


    free_unbounded_int(u);
    free_unbounded_int(ui);
    free_unbounded_int(ul);
    
    return EXIT_SUCCESS;
}