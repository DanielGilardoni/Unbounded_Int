#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include"projet.h"

void free_unbouded_int(unbounded_int u){
    if(u.signe == '*'){
        return;
    }
    chiffre *current = NULL;
    chiffre *previous = NULL;
    previous = u.premier;
    current = previous->suivant;
    while(previous != NULL){
        free(previous);
        previous = current;
        if(current == NULL){
            return;
        }
        current = current->suivant;
    }
}

unbounded_int string2unbounded_int(const char *e){
    int len = strlen(e);
    unbounded_int u;
    u.len = len;
    if(isdigit(*e)){
        u.signe = '+';
    }else if(*e == '+'){
        u.signe = '+';
        u.len -= 1;
    }else if(*e == '-'){
        u.signe = '-';
        u.len -= 1;
    }else{
        u.signe='*';
        return u;
    }
    for(;*e;e++){
        if(!isdigit(*e)){
            u.signe='*';
            return u;
        }
    }
    e=e-u.len;
    u.premier = malloc(sizeof(chiffre));
    if(u.premier == NULL){
        u.signe = '*';
        return u;
    }
    u.premier->precedent = NULL;
    chiffre *current = NULL;
    chiffre *previous = NULL;
    previous = u.premier;
    u.premier->c = *e;
    e++;
    for(;*e;e++){
        current = malloc(sizeof(chiffre));
        if(current == NULL){
            u.signe = '*';
            return u;
        }
        current->c = *e;
        previous->suivant = current;
        current->precedent = previous;
        previous = current;
    }
    u.dernier = previous;
    u.dernier->suivant = NULL;
    return u;
}


int main(){
    unbounded_int u = string2unbounded_int("-125");
    unbounded_int ui = string2unbounded_int("az-125");
    free_unbouded_int(u);
    free_unbouded_int(ui);

    return 0;
}