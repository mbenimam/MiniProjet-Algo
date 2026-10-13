#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

#ifndef _LINKED_LIST_BIBLIO_H_
#define _LINKED_LIST_BIBLIO_H_

#define TITRE_MAX_LENGTH 30
#define AUTEUR_MAX_LENGTH 30
#define TMP_MAX_LENGTH 256


typedef struct LinkedList_biblio{
  s_livre* head;// la tete 
  int nbliv; // nombres de livres
}LinkedList_biblio;

int liste_vide(LinkedList_biblio* Biblio);

LinkedList_biblio* initialise_biblio();

void ajouter_elem_list(LinkedList_biblio* Biblio,
           int num,char* titre, char* auteur);
void afficher_liste(LinkedList_biblio* B);
void lecture_n_entree_list(char* nomfic, int n, LinkedList_biblio *B);

s_livre* recherche_par_num_list(LinkedList_biblio* B, int num);
s_livre* recherche_par_titre_list(LinkedList_biblio* B, char* titre);
s_livre* recherche_par_auteur_list(LinkedList_biblio* B, char* auteur);


void supp_liste_rec(s_livre* t);
void liberer_espace_list(LinkedList_biblio* B);
#endif
