#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "LinkedList_biblio.h"
#include <math.h>

#ifndef _HASHMAP_BIBLIO_H_
#define _HASHMAP_BIBLIO_H_

typedef struct Hashmap_biblio{
  int m;// la taille de la hashmap 
  int nbliv; // nombre de livre dans la map
  LinkedList_biblio** T; // la table 
}Hashmap_biblio;

//int liste_vide(LinkedList_biblio* Biblio);

Hashmap_biblio* initTableHachage(int m);
int hashFunction(char* titre, char* auteur, int m);
void ajouter_elem_map(Hashmap_biblio* Biblio,
           int num,char* titre, char* auteur);

void lecture_n_entree_map(char* nomfic, int n, Hashmap_biblio *B);
void afficher_map(Hashmap_biblio* B);
s_livre* recherche_par_num(Hashmap_biblio* B, int num);
s_livre* recherche_par_titre(Hashmap_biblio* B, char* titre);
s_livre* recherche_par_auteur(Hashmap_biblio* B, char* auteur);

void liberer_espace(Hashmap_biblio* B);
#endif
