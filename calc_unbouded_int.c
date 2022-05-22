#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unbounded_int.h"
#include "unbounded_int.c"

typedef struct variable{
  char* nom;
  unbounded_int val;
} var;

static int Len_l = 0;
static int Cap_l = 4;
static var* l;


static void exit_Erreur(char *e){
  printf("Erreur: %s\n",e);
  exit(EXIT_FAILURE);
}

static void Creer_Liste(){
  l = malloc(sizeof(var) * 4);
  if(l == NULL){
    exit_Erreur("Erreur d'allocation d'espace");
  }
}

static var* get_var(char* nom){
  for (int i = 0; i < Len_l; i++) {
        if((l+i)->nom!=NULL){
            if(strcmp(((l+i)->nom),nom)==0){
                return (l+i);
            }
        }
    }
    return NULL;
}

static int is_num(char* c){
  int len = strlen(c);
  for(int i = 0; i < len; i++){
    if(i != 0 || !(c[i]=='+' || c[i]=='-') || len <= 1)
      break;
    if(!isdigit(c[i])){
      return 0;
    }
  }
  return 1;
}


static void Ajouter_liste(char* nom, unbounded_int val){
  if(Len_l+1 >= Cap_l){
    Cap_l*=2;
    l = realloc(l,sizeof(var)*Cap_l);
  }
  var temp;
  temp.val = val;
  temp.nom = nom;
  l[Len_l] = temp;
  Len_l++;
}


static void run_operation(char* liste, char* v1, char op, char* v2){
  
  var* v = get_var(liste);
  unbounded_int gauche;
  unbounded_int droite;
  unbounded_int val;
  
  if( is_num(v1) ){
    gauche = string2unbounded_int(v1);
  }else{
    var* v = get_var(v1);
    if(v == NULL){
      exit_Erreur("Impossible de trouver la variable");
    }
    gauche = v->val;
  }

  if( is_num(v2) ){
    droite = string2unbounded_int(v2);
  }else{
    if(get_var(v2) == NULL){
      exit_Erreur("Impossible de trouver la variable");
    }
    droite = get_var(v2)->val;
  }

  if(droite.signe == '*' || gauche.signe == '*'){
    exit_Erreur("Erreur assignation de valeur");
  }

  switch(op){
    case '*':
     // val = unbounded_int_produit(gauche,droite);
      break;
    case '+':
      val = unbounded_int_somme(gauche,droite);
      break;
    case '-':
      val = unbounded_int_difference(gauche,droite);
      break;
  }

  if(val.signe == '*'){
    exit_Erreur("Erreur assignation de valeur");
  }

  if(v == NULL){
      Ajouter_liste(liste,val);
  }else{
    v->val = val;
  }

  return;
}


static void interpreteur(FILE* fichier_entré, FILE* fichier_sorti){
    int taille_max=1024;
    char* ligne=malloc(sizeof(char)*taille_max);
    if(ligne == NULL){
      exit_Erreur("Erreur allocation espace");
    }
 /*
    while(fgets(ligne,taille_max,fichier_entré)!=0){
      char* res = malloc(sizeof(char)*taille_max);
      if(res == NULL){
        exit_Erreur("Malloc error.");
      }


      if(sscanf(ligne, "print %s",res)){
        écrire sur le fichier de sortie une ligne sous forme variable = valeur
      }
      else if(sscanf(ligne, "end%s",res)){
        arreter l'execution
      }
      else {
        Continuer la lecture du fichier
      }
    }
    */
}


int main(int argc,char* argv[]){

  FILE* fic_entree;
  FILE* fic_sortie;
  Creer_Liste();
  if(argc>5){
      exit_Erreur("Nombre invalide d'arguments");
  }

  if(argc>1 && strcmp(argv[1],"-i")==0){
      fic_entree=fopen(argv[2],"r");
      if(fic_entree==NULL){
        exit_Erreur("Erreur: Fichier introuvable");
      }

      if(argc>3 && strcmp(argv[3],"-o")==0){
          fic_sortie=fopen(argv[4],"w");
          if(fic_sortie==NULL){
              exit_Erreur("Erreur: Fichier introuvable");
          }
          interpreteur(fic_entree,fic_sortie);
      }

      else{
          interpreteur(fic_entree,stdout);
      }

  }
  else if(argc>1 && strcmp(argv[1],"-o")==0){
      fic_sortie=fopen(argv[2],"w");
      if(fic_sortie==NULL){
        exit_Erreur("Fichier de sorti introuvable");
      }

      if(argc>3 && strcmp(argv[3],"-i")==0){
          fic_entree=fopen(argv[4],"r");
          if(fic_entree==NULL){
            exit_Erreur("Fichier d'entrée introuvable");
          }
          interpreteur(fic_entree,fic_sortie);
      }

      else{
          interpreteur(stdin,fic_sortie);
      }
  }
  
  else{
      interpreteur(stdin,stdout);
  }

  if(fic_entree != NULL)
    fclose(fic_entree);
  if(fic_sortie != NULL)
    fclose(fic_sortie);
  return 0;
}
