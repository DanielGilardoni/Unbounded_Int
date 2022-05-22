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
    char c = '+';
    long long a;
    if(i<0){
        c = '-';
        a = i*(-1);
    } else 
        a = i;
    int b = a%10;
    long count = 0;
    do{
        count++;
        a= a/10;
    }while(a>=10);//on compte le nombre de chiffres dans i
    count++;//Sinon on ne prends pas en compte le dernier chiffre 
    char *long_s = malloc(count+2);//+1 pour mettre le signe et '\0'
    if(long_s == NULL){
        unbounded_int u;
        u.signe = '*';
        return u;
    }
    long_s[0] = c;
    long_s[count+1] = '\0';
    if(i<0)
        a = i*(-1);
    else
        a = i;
    count--;
    for(;count>=0;count--){
        long_s[count+1] = b+'0';
        a = a/10;
        b = a%10;
    }
    return string2unbounded_int(long_s);
}


char *unbounded_int2string(unbounded_int i){
    char *s = malloc(i.len+2);//le nombre de chiffre + le signe + '\0'
    if(s == NULL){
        printf("echec du passage en string");
        return "*";
    }
    if(i.signe !='+' && i.signe != '-'){
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
    if((a.signe != '-' && a.signe != '+') || (b.signe != '-' && b.signe != '+'))
        return -2;
    if(a.signe != b.signe){
        if(a.signe == '-'){
            return -1;
        }
        return 1;
    }
    if(a.len != b.len){
        if(a.len<b.len)
            return -1;
        return 1;
    }
    chiffre *curr_a = a.premier;
    chiffre *curr_b = b.premier;
    while(curr_a != NULL){
        if(curr_a->c < curr_b->c)
            return -1;
        if(curr_a->c > curr_b->c)
            return 1;
        curr_a = curr_a->suivant;
        curr_b = curr_b->suivant;
    }
    return 0;
    
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
    unbounded_int c = ll2unbounded_int(b);
    return unbounded_int_cmp_unbounded_int(a,c);
}

unbounded_int unbounded_int_somme_positive(unbounded_int a, unbounded_int b){//on considére que a et b sont positifs et bien définis
    unbounded_int u;
    u.signe = '+';
    int a_ch, b_ch = 0;
    int len = 0;
    int res = 0;
    int retenue = 0;
    
    chiffre *curr_c;
    chiffre *prev_c = NULL;
    chiffre *curr_a = a.dernier;
    chiffre *curr_b = b.dernier;
    while(curr_a != NULL && curr_b !=NULL){
        len++;
        a_ch = curr_a->c - '0';
        b_ch = curr_b->c - '0';
        res = a_ch + b_ch + retenue;
        curr_c = malloc(sizeof(chiffre));
        if(curr_c == NULL){
            u.signe = '*';
            return u;
        }
        if(curr_a == a.dernier){//au premier tour de boucle pour set u.dernier
            u.dernier = curr_c;
        }
        if(res<10){
            curr_c->c = res+'0';
            retenue = 0;
        }else{
            retenue = 1;
            res = res-10;
            curr_c->c = res+'0';
        }
        curr_c->suivant = prev_c;
        if(prev_c != NULL){
            prev_c->precedent = curr_c;
        }
        prev_c = curr_c;
        curr_a = curr_a->precedent;
        curr_b = curr_b->precedent;
    }
    if(a.len - len ==b.len - len){//si on a tout parcouru
        if(retenue > 0){
            len++;
            curr_c = malloc(sizeof(chiffre));
            if(curr_c == NULL){
                u.signe = '*';
                return u;
            }
            curr_c->c = retenue+'0';
            curr_c->suivant = prev_c;
            prev_c->precedent = curr_c;
            u.premier = curr_c;
        }else
            u.premier = prev_c;
        u.len = len;
        return u;
    }
    if(a.len > len){
        while(curr_a != NULL){
            len++;
            a_ch = curr_a->c -'0';
            res = a_ch + retenue;
            curr_c = malloc(sizeof(chiffre));
            if(curr_c == NULL){
                u.signe = '*';
                return u;
            }
            if(res<10){
                curr_c->c = res+'0';
                retenue = 0;
            }else{
                retenue = retenue;
                res = res-10;
                curr_c->c = res+'0';
            }
            curr_c->suivant = prev_c;
            if(prev_c != NULL){
                prev_c->precedent = curr_c;
            }
            prev_c = curr_c;
            curr_a = curr_a->precedent;
        }
        if(retenue > 0){
            len++;
            curr_c = malloc(sizeof(chiffre));
            if(curr_c == NULL){
                u.signe = '*';
                return u;
            }
            curr_c->c = retenue+'0';
            curr_c->suivant = prev_c;
            prev_c->precedent = curr_c;
            u.premier = curr_c;
        }else
            u.premier = prev_c;
        u.len = len;
        return u;
    }
    while(curr_b != NULL){
        len++;
        b_ch = curr_b->c - '0';
        res = b_ch + retenue;
        curr_c = malloc(sizeof(chiffre));
        if(curr_c == NULL){
            u.signe = '*';
            return u;
        }
        if(res<10){
            retenue = 0;
            curr_c->c = res + '0';
        }else{
            retenue = 1;
            res = res-10;
            curr_c->c = res+'0';
        }
        curr_c->suivant = prev_c;
        if(prev_c != NULL){
            prev_c->precedent = curr_c;
        }
        prev_c = curr_c;
        curr_b = curr_b->precedent;
    }
    if(retenue > 0){
        len++;
        curr_c = malloc(sizeof(chiffre));
        if(curr_c == NULL){
            u.signe = '*';
            return u;
        }
        curr_c->c = retenue+'0';
        curr_c->suivant = prev_c;
        prev_c->precedent = curr_c;
        u.premier = curr_c;
    }else
        u.premier = curr_c;
    u.len = len;
    return u;
}

unbounded_int unbounded_int_difference_positive( unbounded_int a, unbounded_int b){//on considére que la fonction est bien appelé avec a et b positifs et avec a>b
    unbounded_int u;
    u.signe = '+';
    int a_ch, b_ch = 0;
    int len = 0;
    int res = 0;
    int retenue = 0;
    
    chiffre *curr_c;
    chiffre *prev_c = NULL;
    chiffre *curr_a = a.dernier;
    chiffre *curr_b = b.dernier;
    while(curr_a != NULL && curr_b !=NULL){
        len++;
        a_ch = curr_a->c - '0';
        b_ch = curr_b->c - '0';
        b_ch += retenue;
        if(a_ch >= b_ch){
            retenue = 0;
        }else {
            a_ch += 10;
            retenue = 1;
        }
        res = a_ch - b_ch;
        curr_c = malloc(sizeof(chiffre));
        if(curr_c == NULL){
            u.signe = '*';
            return u;
        }
        if(curr_a == a.dernier){//au premier tour de boucle pour set u.dernier
            u.dernier = curr_c;
        }
        curr_c->c = res+'0';
        curr_c->suivant = prev_c;
        if(prev_c != NULL){
            prev_c->precedent = curr_c;
        }
        prev_c = curr_c;
        curr_a = curr_a->precedent;
        curr_b = curr_b->precedent;
    }
    if(a.len - len ==b.len - len){//si on a tout parcouru
        u.premier = prev_c;
        u.len = len;
        return u;
    }//Sinon il faut parcourir la fin de a qui est plus grand
    while(curr_a != NULL){
        len++;

        a_ch = curr_a->c - '0';
        if(a_ch >= retenue){//si la retenue vaut 0 ou si a_ch est >= 1
            retenue = 0;
            res = a_ch - retenue;
        }else {//si la retenue vaut 1 et a_ch 0
            res = 9;
            retenue = 1;
        }
        curr_c = malloc(sizeof(chiffre));
        if(curr_c == NULL){
            u.signe = '*';
            return u;
        }
        curr_c->c = res+'0';
        curr_c->suivant = prev_c;
        if(prev_c != NULL){
            prev_c->precedent = curr_c;
        }
        prev_c = curr_c;
        curr_a = curr_a->precedent;
    }
    u.premier = curr_c;
    u.len = len;
    return u;
}   

unbounded_int remove_zero(unbounded_int u){//cette fonction doit enlever les 0 inutiles
    if(u.signe != '-' && u.signe != '+'){
        return u;
    }
    chiffre* current;
    while(u.premier->c == '0' && u.premier != u.dernier){
        u.len--;
        current = u.premier;
        u.premier = u.premier->suivant;
        u.premier->precedent = NULL;
        free(current);
    }
    return u;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    unbounded_int sum;
    if((a.signe != '-' && a.signe != '+') || (b.signe != '-' && b.signe != '+')){
        sum.signe = '*';
        return sum;
    }
    int res = unbounded_int_cmp_unbounded_int(a,b);
    if(a.signe == '+' && b.signe == '+'){
        sum = unbounded_int_somme_positive(b,a);
        sum.signe = '+';
    }else if(a.signe == '-' && b.signe == '-'){
        sum = unbounded_int_somme_positive(b,a);
        sum.signe = '-';
    }else if(a.signe == '-'){
        a.signe = '+';
        res = unbounded_int_cmp_unbounded_int(a,b);
        if(res == -1){
            sum = unbounded_int_difference_positive(b,a);
            sum.signe = '+';
        }else{
            sum = unbounded_int_difference_positive(a,b);
            sum.signe = '-';
        }
        a.signe = '-';
    }else{
        b.signe = '+';
        res = unbounded_int_cmp_unbounded_int(a,b);
        if(res == -1){
            sum = unbounded_int_difference_positive(b,a);
            sum.signe = '-';
        }else{
            sum = unbounded_int_difference_positive(a,b);
            sum.signe = '+';
        }
        b.signe = '-';
    }
    sum = remove_zero(sum);
    return sum;
}

unbounded_int unbounded_int_difference( unbounded_int a, unbounded_int b){
    unbounded_int diff;
    if((a.signe != '-' && a.signe != '+') || (b.signe != '-' && b.signe != '+')){
        diff.signe = '*';
        return diff;
    }
    int res = unbounded_int_cmp_unbounded_int(a,b);
    if(a.signe == '+' && b.signe == '+'){
        if(res == -1){//si a < b
            diff = unbounded_int_difference_positive(b,a);
            diff.signe = '-';
        }else{
            diff = unbounded_int_difference_positive(a,b);
            diff.signe = '+';
        }
    }else if(a.signe == '-' && b.signe == '-'){
        if(res == -1){
            diff = unbounded_int_difference_positive(a,b);
            diff.signe = '-';
        }else{
            diff = unbounded_int_difference_positive(b,a);
            diff.signe = '+';
        }
    }else if(a.signe == '-'){
        diff.signe = '-';
        diff = unbounded_int_somme_positive(a,b);
    }else{
        diff.signe = '+';
        diff = unbounded_int_somme_positive(a,b);
    }
    diff = remove_zero(diff);
    return diff;
}

/*unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b){
    char *str_a = unbounded_int2string(a);
    char *str_b = unbounded_int2string(b);
    str_a++;
    str_b++;
    int n = strlen(str_a);
    int m = strlen(str_b);
    char *str_c = calloc(m + n + 1,1);
    int i=0;
    int r=0;
    for( int j = 0; j < m; j++){ 
        r = 0;
        if( str_b[j] == '0' )
            continue;
        for(i=0; i < n; i++ ){ 
            int v = str_c[i+j]-'0' + (str_a[i]+'0')*(str_b[j]+'0') + r;
            str_c[i+j] = (v % 10)+'0';
            r = v / 10;
            printf(" la : %c\n", str_c[i+j]);
        }
        str_c[j+n] = r+'0';
    }
    return string2unbounded_int(str_c);
}*/
