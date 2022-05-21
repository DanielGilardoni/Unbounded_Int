#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include "unbounded_int.h"

void free_unbounded_int(unbounded_int u){
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
        e++;
    }else if(*e == '-'){
        u.signe = '-';
        u.len -= 1;
        e++;
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
    e-=u.len;
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
    long long a = i;
    int b = a%10;
    long count = 0;
    //printf("\n");
    do{
        count++;
        a= a/10;
        //printf("a : %lld\n", a);
    }while(a>=10);//on compte le nombre de chiffres dans i
    count++;//Sinon on ne prends pas en compte le dernier chiffre 
    //printf("count : %ld\n",count);
    char *long_s = malloc(sizeof(char)*count+1);//+1 pour mettre '\0'
    if(long_s == NULL){
        //printf("yo");
        unbounded_int u;
        u.signe = '*';
        return u;
    }
    long_s[count] = '\0';
    a = i;
    count--;
    for(;count>=0;count--){
        //printf("b : %d\n",b);
        long_s[count] = (char)(b+'0');
        //printf("long_s : %c\n", *long_s);
        a = a/10;
        b = a%10;
    }
    //printf("%s",long_s);
    return string2unbounded_int(long_s);
}


char *unbounded_int2string(unbounded_int i){
    char *s = malloc(i.len+2);//le nombre de chiffre + le signe + '\0'
    if(s == NULL){
        printf("echec du passage en string");
        exit(1);
    }
    if(i.signe =='*'){
        s[0]='*';
        s[1]='\0';
        return s;
    }
    if(i.signe=='+'){
        s[0]='+';
    }else{
        s[0]='-';
    }
    chiffre *current = i.premier;
    for(int j =1;current !=NULL && j<=i.len;current=current->suivant, j++){
        s[j]=current->c;
    }
    s[i.len+1]='\0';
    return s;

}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){
    if(a.signe == '*' || b.signe == '*')
        return 0;
    
}
