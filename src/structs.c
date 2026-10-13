#include "structs.h"
#include <stdio.h>
#include <string.h>



s_livre* creer_livre(int num, char* titre, char* auteur){
  
  // cette fonction cree et initialise un livre

  // allocation
  s_livre* nouv = (s_livre*)malloc(sizeof(s_livre));
  

  // initialisation
  nouv->num = num;
  nouv->titre = strdup(titre);
  nouv->auteur = strdup(auteur);
  nouv->suiv = NULL;

  return nouv;
}
void afficher_livre(s_livre* l){
   printf("le livre cherché est : %d %s %s\n", l->num, l->titre, l->auteur);
}