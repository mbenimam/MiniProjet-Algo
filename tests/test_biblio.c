/*
 * Unit tests for the library catalogue.
 *
 * Build and run with `make test` from the repository root.
 * Each CHECK compares real values; the program exits non-zero if any check fails.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "entree_sortie.h"
#include "LinkedList_biblio.h"
#include "Hashmap_biblio.h"

static int checks = 0;
static int failures = 0;

#define CHECK(cond)                                                    \
  do {                                                                 \
    checks++;                                                          \
    if (!(cond)) {                                                     \
      failures++;                                                      \
      fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);  \
    }                                                                  \
  } while (0)

#define FIXTURE "build/test_fixture.txt"

/* writes a fixture file with the given content, returns 0 on success */
static int write_fixture(const char *content) {
  FILE *f = fopen(FIXTURE, "w");
  if (!f) return -1;
  fputs(content, f);
  fclose(f);
  return 0;
}

static void test_creer_livre_copies_strings(void) {
  char titre[] = "Dune";
  char auteur[] = "Herbert";
  s_livre *l = creer_livre(1, titre, auteur);

  /* changing the caller's buffers must not change the stored book */
  strcpy(titre, "XXXX");
  strcpy(auteur, "YYYYYYY");

  CHECK(l->num == 1);
  CHECK(strcmp(l->titre, "Dune") == 0);
  CHECK(strcmp(l->auteur, "Herbert") == 0);
  CHECK(l->suiv == NULL);

  free(l->titre);
  free(l->auteur);
  free(l);
}

static void test_list_insert_and_count(void) {
  LinkedList_biblio *B = initialise_biblio();
  CHECK(liste_vide(B));
  CHECK(B->nbliv == 0);

  ajouter_elem_list(B, 1, "A", "x");
  ajouter_elem_list(B, 2, "B", "y");
  ajouter_elem_list(B, 3, "C", "z");

  CHECK(!liste_vide(B));
  CHECK(B->nbliv == 3);
  /* insertion is at the head, so the last inserted book comes first */
  CHECK(B->head->num == 3);
  CHECK(B->head->suiv->num == 2);
  CHECK(B->head->suiv->suiv->num == 1);
  CHECK(B->head->suiv->suiv->suiv == NULL);

  liberer_espace_list(B);
}

static void test_list_search(void) {
  LinkedList_biblio *B = initialise_biblio();
  ajouter_elem_list(B, 1, "Dune", "Herbert");
  ajouter_elem_list(B, 2, "Emma", "Austen");

  s_livre *l = recherche_par_num_list(B, 2);
  CHECK(l != NULL && strcmp(l->titre, "Emma") == 0);
  CHECK(recherche_par_num_list(B, 9) == NULL);

  l = recherche_par_titre_list(B, "Dune");
  CHECK(l != NULL && l->num == 1);
  CHECK(recherche_par_titre_list(B, "Ulysse") == NULL);

  l = recherche_par_auteur_list(B, "Austen");
  CHECK(l != NULL && l->num == 2);
  CHECK(recherche_par_auteur_list(B, "Joyce") == NULL);

  liberer_espace_list(B);
}

static void test_list_reader_respects_n(void) {
  CHECK(write_fixture("1 Dune Herbert\n2 Emma Austen\n3 Ulysse Joyce\n") == 0);

  LinkedList_biblio *B = initialise_biblio();
  lecture_n_entree_list(FIXTURE, 2, B);

  /* only the first two lines are read */
  CHECK(B->nbliv == 2);
  CHECK(B->head->num == 2);
  CHECK(recherche_par_num_list(B, 3) == NULL);

  liberer_espace_list(B);
  remove(FIXTURE);
}

static void test_reader_primitives(void) {
  CHECK(write_fixture("42 -7 3.25 hello\n") == 0);
  FILE *f = fopen(FIXTURE, "r");
  CHECK(f != NULL);
  if (f) {
    CHECK(GetEntier(f) == 42);
    CHECK(GetEntier(f) == -7);
    CHECK(GetReel(f) == 3.25);
    char word[32];
    GetChaine(f, sizeof word, word);
    CHECK(strcmp(word, "hello") == 0);
    fclose(f);
  }
  remove(FIXTURE);
}

static void test_hash_range_and_determinism(void) {
  const int m = 1000;
  int ok = 1;
  for (int i = 0; i < 500; i++) {
    char titre[32], auteur[32];
    snprintf(titre, sizeof titre, "T%dxyz", i);
    snprintf(auteur, sizeof auteur, "A%d", i * 7);
    int h = hashFunction(titre, auteur, m);
    if (h < 0 || h >= m) ok = 0;
    if (h != hashFunction(titre, auteur, m)) ok = 0;
  }
  CHECK(ok);
  CHECK(hashFunction("", "", 10) >= 0 && hashFunction("", "", 10) < 10);
}

static void test_hashmap_insert_counts(void) {
  const int m = 16;
  Hashmap_biblio *H = initTableHachage(m);
  CHECK(H != NULL);
  CHECK(H->m == m);

  for (int i = 0; i < 50; i++) {
    char titre[32], auteur[32];
    snprintf(titre, sizeof titre, "Titre%d", i);
    snprintf(auteur, sizeof auteur, "Auteur%d", i);
    ajouter_elem_map(H, i, titre, auteur);
  }

  /* the map counts every book, and so do its buckets together */
  int in_buckets = 0;
  for (int i = 0; i < m; i++) in_buckets += H->T[i]->nbliv;
  CHECK(H->nbliv == 50);
  CHECK(in_buckets == 50);

  liberer_espace(H);
}

static void test_hashmap_reader(void) {
  CHECK(write_fixture("1 Dune Herbert\n2 Emma Austen\n3 Ulysse Joyce\n") == 0);
  Hashmap_biblio *H = initTableHachage(8);
  lecture_n_entree_map(FIXTURE, 3, H);
  CHECK(H->nbliv == 3);
  liberer_espace(H);
  remove(FIXTURE);
}

int main(void) {
  test_creer_livre_copies_strings();
  test_list_insert_and_count();
  test_list_search();
  test_list_reader_respects_n();
  test_reader_primitives();
  test_hash_range_and_determinism();
  test_hashmap_insert_counts();
  test_hashmap_reader();

  printf("%d checks, %d failed\n", checks, failures);
  return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
