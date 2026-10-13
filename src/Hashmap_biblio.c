#include "Hashmap_biblio.h"





Hashmap_biblio* initTableHachage(int m){
    Hashmap_biblio* B =  (Hashmap_biblio*)malloc(sizeof(Hashmap_biblio));
    if(!B){
        fprintf(stderr, "Erreur allocation hashmap\n");
        return NULL;
    }

    B->m = m;
    B->nbliv = 0;
    B->T = (LinkedList_biblio**)malloc(sizeof(LinkedList_biblio*)*m);
    int i;
    for(i=0; i<m; i++){
        (B->T)[i] = initialise_biblio();
    }
    if(!(B->T)){
        fprintf(stderr, "Erreur allocation T de hashmap\n");
        return NULL;
    }

    return B;

}

void afficher_map(Hashmap_biblio* B){
    if(B){
        int i = 0;
        for(i=0; i<B->m; i++){
            afficher_liste((B->T)[i]);
        }
    }
}
int hashFunction(char* titre, char* auteur, int m){
    // fonction de kanuth
    double A = ((double)(sqrt(5) - 1.0))/(double)2.0;
    int k = 0; // la somme des caracteres titre auteur;
    int i;
    for(i=0; titre[i]; i++) k += (unsigned char)titre[i];
    for(i=0; auteur[i]; i++) k += (unsigned char)auteur[i];
    return floor(m*(k*A - floor(k*A)));

}
void ajouter_elem_map(Hashmap_biblio* Biblio,
           int num,char* titre, char* auteur){

        // on recherche son emplacement dans la map
        int indice = hashFunction(titre, auteur, Biblio->m);
        LinkedList_biblio* liste_temp = (Biblio->T)[indice];
        // on ajoute a la liste elle traite le cas ou la liste est vide
        ajouter_elem_list(liste_temp, num, titre, auteur);
        // on incremente le compteur de la map
        Biblio->nbliv++;

}

void lecture_n_entree_map(char* nomfic, int n, Hashmap_biblio *B){
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
           ajouter_elem_map(B, tmp_num, tmp_titre, tmp_auteur);
           cpt++;
        }
    }
    fclose(f);
}

s_livre* recherche_par_num(Hashmap_biblio* B, int num){return NULL;}
s_livre* recherche_par_titre(Hashmap_biblio* B, char* titre){return NULL;}
s_livre* recherche_par_auteur(Hashmap_biblio* B, char* auteur){return NULL;}

void liberer_espace(Hashmap_biblio* B){
    if(!B) return;
    int i;
    for(i=0; i<B->m; i++){
        liberer_espace_list((B->T)[i]);
    }
    free(B->T);
    free(B);
}