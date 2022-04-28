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

unbounded_int ll2unbounded_int(long long i){//Sinon transformer en unbounded directe
    int a = i;
    int b = a%10;
    int count = 0;
    do{
        count++;
        a= a/10;
    }while(a>10);
    char *long_s = malloc(sizeof(char)*count);
    if(long_s == NULL){
        unbounded_int u;
        u.signe = '*';
        return u;
    }
    long_s += count-1;
    a = i;
    for(;count>0;count--){
        *long_s = (char)b;
        a = a/10;
        b = a%10;
        if(a != 0)
            long_s--;//Sinon à la derniére iteration de la boucle on revient trop en arriére je crois
    }
    return string2unbounded_int(long_s);
}


int main(){
    unbounded_int u = string2unbounded_int("-125");
    unbounded_int ui = string2unbounded_int("az-125");
    long long i = 102568;
    unbounded_int ul = ll2unbounded_int(i);
    free_unbouded_int(u);
    free_unbouded_int(ui);
    free_unbouded_int(ul);

    return 0;
}