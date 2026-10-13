#include <stdio.h>
#include <stdlib.h>
#include "entree_sortie.h"
#include "LinkedList_biblio.h"
#include "Hashmap_biblio.h"
#include <time.h>

void menu(){

  printf("\t1-Lire n lignes fichier\n");
  printf("\t2-Afficher la liste\n");
  printf("\t3-Rechercher un ouvrage par son num\n");
  printf("\t4-Rechercher un ouvrage par son titre\n");
  printf("\t5-Rechercher un ouvrage par son auteur\n");
  printf("\t6-Inserer un nouvel ouvrage\n\n");
  printf("\tVeuillez choisir une option : ");
}

/*void run_tests(char* logFile, char* Fichier_test){
  /*
    outputs a file containing n collumns 
    1- taille de l'entree
    2- recherche par num qui n'existe pas dans la liste (pire cas) 
    3- recherche par titre qui n'existe pas dans la liste (pire cas) 
    4-recherche par auteur qui n'existe pas dans la liste (pire cas) 
    5- espace memoire
    

    for the purpous of the test, we will read the full doc
    then limit the list by a counter so that we don't read the whole 
    doc everytime
  
  
  int n = 10; // la taille de la bibliotheque
  //int nbr_tests = 5;// nbr de tests a faire , utile pour calculer la moyenne
  //int somme_temp = 0;
  clock_t initial;
  clock_t final;
  double deltatime[3];


  FILE* f = fopen(logFile, "w");
  // on cree une bibliotheque 

  // on charge la totalité de la bibliotheque
  //lecture_n_entree(Fichier_test, 100000, B);

  // faire les tests pour des valeur de n < 100000 avec des pas de plus en plus grands
  LinkedList_biblio* B ;
  while(n <= 100000){
    printf("\nTest avec n = %d\n", n);
    // pour faire le test pour different valeurs de n
    B = initialise_biblio();
    lecture_n_entree_map(Fichier_test, n, B);
    // temps d'execution de recherche par num
    initial = clock();
    recherche_par_num(B, -5);
    final = clock();
    deltatime[0] = ((double)(final - initial))/CLOCKS_PER_SEC;
    
     // temps d'execution de recherche par titre
    initial = clock();
    recherche_par_titre(B, "OVOYEEOXGNNQCSQQRr");
    final = clock();
    deltatime[1] = ((double)(final - initial))/CLOCKS_PER_SEC;

    // temps d'execution de recherche par auteur
    initial = clock();
    recherche_par_auteur(B, "awfexmhdbhZ");
    final = clock();
    deltatime[2] = ((double)(final - initial))/CLOCKS_PER_SEC;



    // on enregistre ca dans le fichier
    fprintf(f, "%d %.5f %.5f %.5f\n", n, deltatime[0], deltatime[1], deltatime[2]);
    n += 1000;
    liberer_espace(B);
  }

  fclose(f);

}*/
int main(int argc, char** argv){


  // si le format d'appel du programme est faux on sort
  if(argc != 4){
    printf("Erreur format : %s <NomFichierBiblio.txt> <1: liste, 2: map> <nblignes>\n", argv[0]);
    return 1;
  }




    /*
    argc contient le nombre de parametres
    argv contient un tableau de parametres 
  */

  int ch;// pour contenir le choix de l'utilisateur
  char* nomfic;// le nom du fichier de livres
  int nlignes;

  int typestruct;// nombre de lignes 
  s_livre* tmp;
  nomfic = strdup(argv[1]);// on copie le nom du ficher
  typestruct = atoi(argv[2]);// on recupere le nb lignes voulu
  nlignes = atoi(argv[3]);
  Hashmap_biblio* B;
  LinkedList_biblio* B_liste;
  


  if(typestruct == 2){
    B = initTableHachage(1000);
  }else{
    B_liste = initialise_biblio();
  }
  

if(typestruct == 2){
  do{
    menu();// on afficher le menu
    scanf(" %d", &ch);// on lit le choix de l'utilisateur

    switch(ch){
      case 0:  printf("Au revoir!\n");
        break; 
      case 1:  printf("Lecture...\nnomfichier : %s , nlignes : %d\n", nomfic, nlignes);
               lecture_n_entree_map(nomfic, nlignes, B);// Lire n lignes
        break;
      case 2:  afficher_map(B);// Affichage liste
        break;
      case 3:  ;
                 tmp= recherche_par_num(B, 4);
                if(tmp)
                  afficher_livre(tmp);
                 // Recherche par num
        break;
      case 4:  ;tmp = recherche_par_titre(B, "UKWAGM");
                if(tmp)
                  afficher_livre(tmp);// Recherche par titre
        break;
      case 5:  ;tmp = recherche_par_auteur(B, "gspqoqm");
                if(tmp)
                  afficher_livre(tmp);// Recherche par auteur
        break;
      case 6: //Insert_element(B);// Insertion element
      case 7: //run_tests("LinkedList.log", nomfic);// run tests
        break;
      default:
        printf("\nErreur: entrez un choix valide\n");
        break;
    }
    // tant qu'on entre quelque chose
  }while(ch != 0);

}else{
  do{
    menu();// on afficher le menu
    scanf(" %d", &ch);// on lit le choix de l'utilisateur

    switch(ch){
      case 0:  printf("Au revoir!\n");
        break; 
      case 1:  printf("Lecture...\nnomfichier : %s , nlignes : %d\n", nomfic, nlignes);
               lecture_n_entree_list(nomfic, nlignes, B_liste);// Lire n lignes
        break;
      case 2:  afficher_liste(B_liste);// Affichage liste
        break;
      case 3:  ;
                 tmp= recherche_par_num_list(B_liste, 4);
                if(tmp)
                  afficher_livre(tmp);
                 // Recherche par num
        break;
      case 4:  ;tmp = recherche_par_titre_list(B_liste, "UKWAGM");
                if(tmp)
                  afficher_livre(tmp);// Recherche par titre
        break;
      case 5:  ;tmp = recherche_par_auteur_list(B_liste, "gspqoqm");
                if(tmp)
                  afficher_livre(tmp);// Recherche par auteur
        break;
      case 6: //Insert_element(B);// Insertion element
      case 7: //run_tests("LinkedList.log", nomfic);// run tests
        break;
      default:
        printf("\nErreur: entrez un choix valide\n");
        break;
    }
    // tant qu'on entre quelque chose
  }while(ch != 0);
}




  // on libere la bibliotheque et le nom de fichier avant de sortir
  if(typestruct == 2){
    liberer_espace(B);
  }else{
    liberer_espace_list(B_liste);
  }
  free(nomfic);

  return 0;
}
