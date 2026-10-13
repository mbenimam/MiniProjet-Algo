# MiniProjet-Algo

A C library catalogue that stores the same books in a **linked list** and in a **hash map**, so you can see the trade-off between the two in code, with tests and sanitizers behind both.

| | |
| --- | --- |
| **What** | Loads a file of books (number, title, author) into a linked list or a 1000-bucket hash map, and shows them from a menu. |
| **For** | A university data-structures course project, revived as a clean, testable C codebase. |
| **Stack** | C (gnu11), GNU make, GitHub Actions, AddressSanitizer and UBSan. |
| **Result** | 36 unit checks; 7 of 10 deliberately reintroduced bugs fail a plain check, and the other 3 are caught by the sanitizers. Builds with GCC and Clang. |
| **Read next** | [How the hash map places a book](#how-the-hash-map-places-a-book), with a worked example from the code. |

![How books are placed in the hash map](docs/assets/diagrams/hashmap-insert.png)

## Context

The project is a university course exercise that builds a book catalogue in C with two ways of storing the books. The original code from 2019 did not compile with a current compiler, had no tests, and had three empty search functions in the hash map. I brought it back to a working state so that the comparison can be run and checked.

The difficulty is the one every catalogue has: a lookup by title or author must not walk the whole collection. A linked list is simple and cheap to insert into, but a lookup walks it from the head. A hash map jumps to one bucket first, at the cost of handling collisions.

## How the hash map places a book

A hash map has *m* buckets. A **hash function** turns each book's title and author into a bucket index between 0 and *m*−1. Each bucket is a linked list, so two books that land in the same bucket are both kept. This is called **separate chaining**.

The hash function here adds up the character codes of the title and the author into a number *k*, then uses multiplicative hashing (Knuth): it keeps the fraction of *k* × 0.618… and scales it to the table size.

```c
double A = (sqrt(5) - 1.0) / 2.0;             // about 0.618
for (int i = 0; titre[i]; i++) k += (unsigned char)titre[i];
for (int i = 0; auteur[i]; i++) k += (unsigned char)auteur[i];
return floor(m * (k * A - floor(k * A)));     // bucket in 0 .. m-1
```

Worked example with the first four books of the sample file and *m* = 8, output from the repository's own functions:

| Book | Title | Author | *k* | Bucket |
|---|---|---|---|---|
| 0 | WLRBBMQBHCDARZOWK | yhidd | 1816 | 2 |
| 1 | SCDXRJ | owfrx | 1028 | 2 (collision) |
| 2 | JYBLD | efsarcbynecd | 1630 | 3 |
| 3 | GGXXPK | orellnmpap | 1555 | 0 |

Books 0 and 1 share bucket 2, so bucket 2 holds two books, with the newer one in front. Inserting a book is always at the head of its bucket, so an insert does not walk the chain.

![The linked list grows at the head](docs/assets/diagrams/list-head-insert.png)

*The linked list, for comparison: each new book goes in front, so searching it means walking from the head.*

## What it does

- **Read** *n* books from a file into the chosen structure.
- **Show** the list, or every bucket of the map.
- **Search** by number, title or author (list backend; see Status for the map).

## Quick start

Requirements: a C compiler (`gcc` or `clang`) and `make`.

```sh
make                              # builds ./MiniProjet
make test                         # 36 unit checks, then a smoke run of each backend
make test-asan                    # the same checks under AddressSanitizer and UBSan

./MiniProjet GdeBiblio.txt 1 100  # linked list, first 100 books
./MiniProjet GdeBiblio.txt 2 100  # hash map, first 100 books
```

The three arguments are the catalogue file, the structure (`1` = linked list, `2` = hash map), and the number of books to read.

| Menu option | Action |
|---|---|
| `1` | Read *n* books from the file into the structure |
| `2` | Show the list (or every bucket of the map) |
| `3` | Search by number (key fixed in `main.c`: `4`) |
| `4` | Search by title (key fixed in `main.c`: `"UKWAGM"`) |
| `5` | Search by author (key fixed in `main.c`: `"gspqoqm"`) |
| `0` | Quit |

## Testing and verification

- `make test` runs 36 checks written as plain comparisons (no framework). They cover the book constructor, list insert order and search, the reader's record limit, the number readers, the hash range, and the map counts.
- To check that the tests catch real bugs, I reintroduced ten bugs one at a time in a scratch copy. Seven fail a plain check. Three (a hash-function read past the end of a string, and two memory leaks) are caught only by `make test-asan`.
- `make test-asan` runs the same checks under AddressSanitizer and UBSan, and reports no errors.
- A GitHub Actions workflow (`.github/workflows/build.yml`) runs the build, the tests and the sanitizer run on every push and pull request.

## Status

- **Works:** the linked list (insert, show, search by number, title and author), the hash map (insert, show, counts), and the file readers for both.
- **Not built yet:** search in the hash map. Its `recherche_par_*` functions return `NULL`, so in map mode options `3` to `5` print nothing.
- **Not built yet:** inserting a new book from the menu (option `6`).
- **Limitation:** search keys in options `3` to `5` are fixed values in `main.c`, not typed by the user.

## Data file

One book per line: `number title author`, with single-word titles and authors of at most 29 characters. `GdeBiblio.txt` is a sample catalogue of 100,000 books with randomly generated titles and authors. It is generated data, so the numbers in this README describe its behaviour, not a real collection.

## Project layout

```
src/
  main.c                 menu and program entry point
  structs.[ch]           book record and its constructor
  LinkedList_biblio.[ch] linked-list backend and reader
  Hashmap_biblio.[ch]    hash-map backend, hash function and reader
  entree_sortie.[ch]     word-by-word reading helpers
tests/test_biblio.c      unit tests
docs/assets/diagrams/    Excalidraw sources and PNGs for this README
Mini_Projet.pdf          the course assignment brief
GdeBiblio.txt            sample catalogue
```
