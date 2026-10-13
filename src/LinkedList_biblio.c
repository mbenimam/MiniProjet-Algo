#include "LinkedList_biblio.h"





LinkedList_biblio* initialise_biblio(){
  // cette fonction cree une LinkedList_biblio et l'initialise

  // allocation
  LinkedList_biblio* nouv = (LinkedList_biblio*)malloc(sizeof(LinkedList_biblio));
  
  //initialisation
  nouv->head = NULL;
  nouv->nbliv = 0;

  // et on retourne
  return nouv;
}

int liste_vide(LinkedList_biblio* Biblio){
  // cette fonction verifie si la liste est vide
  return (Biblio->head == NULL);
}


void ajouter_elem_list(LinkedList_biblio* Biblio,
           int num,char* titre, char* auteur){
  // cette fonction ajoute un element au debut de la liste
  
  // creation de l'element
  s_livre* nouv = creer_livre(num, titre, auteur);
  // cas ou la liste est vide
  if(liste_vide(Biblio)){
    Biblio->head = nouv;
  }else{
    nouv->suiv = Biblio->head;// on garde l'ancienne tete
    Biblio->head = nouv;
  }
  Biblio->nbliv += 1; // on incremente le compteur de livres

}

void lecture_n_entree_list(char* nomfic, int n, LinkedList_biblio *B){
    // cette fonction lit dans un fichier nomfic
    // n lignes contenant des info de livre (num, titre, auteur)
    
    int cpt = 0; // compteur de lignes deja lus
    int tmp_num; // tmp num
    char tmp_titre[TITRE_MAX_LENGTH];// tmp titre
    char tmp_auteur[AUTEUR_MAX_LENGTH];// tmp auteur

    // on ouvre le fichier
    FILE* f = fopen(nomfic, "r");
    if(!f){
        fprintf(stderr, "Erreur de Lecture du fichier %s\n", nomfic);
        return;
    }
    // on cree un buffer pour contenir les lignes lus
    char Buffer[TMP_MAX_LENGTH];

    // lecture des lignes
    while((cpt < n) && (fgets(Buffer, TMP_MAX_LENGTH, f) != NULL)){
        // lecture du format
        if(sscanf(Buffer, "%d %29s %29s", &tmp_num, tmp_titre, tmp_auteur) != 3){
            fprintf(stderr, "Erreur de Format\n");
            return;
        }else{
           ajouter_elem_list(B, tmp_num, tmp_titre, tmp_auteur);
           cpt++;
        }
    }
    fclose(f);

}

void afficher_liste(LinkedList_biblio* B){
    //cette fonction affiche les livres dans la liste
    
    //on initialise le pointeur a la tete
    s_livre* tmp = B->head;
    printf("La liste contient : %d livres\n\n", B->nbliv);
    // tan que la liste n'est pas fini
    while(tmp != NULL){
        // on affiche le livre
        printf("%d %s %s\n", tmp->num, tmp->titre, tmp->auteur);
        // on bouge le temp
        tmp = tmp->suiv;
    }

    //printf("\nFin de l'affichage de la liste\n");

}
s_livre* recherche_par_num_list(LinkedList_biblio* B, int num){
    /*
        cette fonction permet de chercher un livre par son num
    */    
    if(!B){
        fprintf(stderr, "\nErreur il faut initialsé la bibliotheque");
        return NULL;
    }
    s_livre* temp = B->head;

    if(!temp){
        // l'element n'xiste pas dans la liste
        return NULL;
    }
    while(temp){
        if(temp->num == num){
            // on a retrouvé l'element , on sort
            return temp;
        }
        temp = temp->suiv;
    }

    return NULL;
}
s_livre* recherche_par_titre_list(LinkedList_biblio* B, char* titre){/*
        cette fonction permet de chercher un livre par son num
    */    
    if(!B){
        fprintf(stderr, "\nErreur il faut initialsé la bibliotheque");
        return NULL;
    }
    s_livre* temp = B->head;

    if(!temp){
        // l'element n'xiste pas dans la liste
        return NULL;
    }
    while(temp){
        if(!strcmp(temp->titre ,titre)){
            // on a retrouvé l'element , on sort
            return temp;
        }
        temp = temp->suiv;
    }

    return NULL;}
s_livre* recherche_par_auteur_list(LinkedList_biblio* B, char* auteur){/*
        cette fonction permet de chercher un livre par son num
    */    
    if(!B){
        fprintf(stderr, "\nErreur il faut initialsé la bibliotheque");
        return NULL;
    }
    s_livre* temp = B->head;

    if(!temp){
        // l'element n'xiste pas dans la liste
        return NULL;
    }
    while(temp){
        if(!strcmp(temp->auteur , auteur)){
            // on a retrouvé l'element , on sort
            return temp;
        }
        temp = temp->suiv;
    }

    return NULL;}

void supp_liste_rec(s_livre* t){
  // libere tous les livres a partir de t (iteratif pour ne pas depasser la pile)
  while(t){
    s_livre* suiv = t->suiv;
    free(t->titre);
    free(t->auteur);
    free(t);
    t = suiv;
  }
}

void liberer_espace_list(LinkedList_biblio* B){
  if(B){
    supp_liste_rec(B->head);
    free(B);
  }
}
