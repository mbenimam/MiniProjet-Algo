# Library catalogue (data structures)

![C gnu11](https://img.shields.io/badge/C-gnu11-blue)
![tests 36 checks](https://img.shields.io/badge/tests-36%20checks-brightgreen)

**One book catalogue, two ways to store it, and a hash map that places every book in a bucket.** This is a C project I brought back to life: it builds with a modern compiler, its 36 checks pass, and it is set up to run its sanitizers on every push.

| | |
| --- | --- |
| **What** | A library catalogue that loads books (number, title, author) into a linked list or a 1000-bucket hash map. |
| **For** | A university data-structures project, now a clean and testable C codebase. |
| **Stack** | C (gnu11), GNU make, GitHub Actions, AddressSanitizer and UBSan. |
| **Result** | 36 unit checks. Of 10 bugs I put back on purpose, 7 fail a plain check and the other 3 are caught by the sanitizers. Builds with GCC and Clang. |
| **Start here** | [The hash map in one example](#the-hash-map-in-one-example) |

![How books are placed in the hash map](docs/assets/diagrams/hashmap-insert.png)

## What the project is

A library catalogue is a collection of books. Each book has three fields: a **number** that identifies it, a **title**, and an **author**. The program loads books from a text file, one book per line, and lets you look them up.

The catalogue can be stored in two data structures. The question the project asks is which one suits which kind of lookup, and how the answer changes as the catalogue grows.

The operations are: read *n* books from the file, show them, find a book by its number, its title or its author, and insert a new book.

## Words used in this README

- **Data structure**: a way of arranging data in memory so that certain operations are fast.
- **Linked list**: a chain of nodes, each holding one book and a pointer to the next node. The list keeps a pointer to its first node, the **head**. Adding at the head is quick. Finding a book means walking the chain from the head, one node at a time.
- **Hash map (hash table)**: an array of **buckets**. A hash function turns a book's details into a bucket number, so a lookup goes straight to one bucket instead of checking every book.
- **Hash function**: a function that turns some data into a number in a fixed range. The same input must always give the same number.
- **Bucket**: one slot in the array of a hash map. Here each bucket is itself a linked list.
- **Collision**: two different books that the hash function sends to the same bucket.
- **Separate chaining**: how collisions are handled here. Each bucket keeps all its books in a list, so none is lost.
- **Lookup**: finding a book by one of its fields.
- **Worst case**: the most work an operation can need. It matters when a program must stay fast on unlucky inputs.

## Why the two structures differ

Both structures can store the books. What differs is how much work a lookup takes. In a linked list, a lookup may check every book. In a hash map, a lookup checks only the books in one bucket, as long as the hash function spreads the books evenly. In the worst case, every book lands in one bucket and the map becomes a list again. Inserting costs the same in both: a quick head insertion. The map's lookup is not built yet, so this comparison describes the design. It is not a measurement.

## The hash map in one example

A hash map is an array of *m* buckets. A **hash function** turns each book's title and author into a bucket number from 0 to *m*−1. Each bucket is a linked list, so two books can share a bucket and both are kept. That trick is called **separate chaining**, and it is the reason the map never refuses an insert.

The hash function here adds up the character codes of the title and the author into a number *k*. Then it uses Knuth's multiplicative hashing: it multiplies *k* by 0.618…, keeps the fraction, and scales it to the number of buckets.

Excerpt from `src/Hashmap_biblio.c`, shortened:

```c
double A = (sqrt(5) - 1.0) / 2.0;             // about 0.618
for (int i = 0; titre[i]; i++) k += (unsigned char)titre[i];
for (int i = 0; auteur[i]; i++) k += (unsigned char)auteur[i];
return floor(m * (k * A - floor(k * A)));     // a bucket from 0 to m-1
```

Here is the real output for the first four books of the sample file, with *m* = 8. These numbers come from the repository's own functions:

| Book | Title | Author | *k* | Bucket |
|---|---|---|---|---|
| 0 | WLRBBMQBHCDARZOWK | yhidd | 1816 | 2 |
| 1 | SCDXRJ | owfrx | 1028 | 2, a collision |
| 2 | JYBLD | efsarcbynecd | 1630 | 3 |
| 3 | GGXXPK | orellnmpap | 1555 | 0 |

Books 0 and 1 land in the same bucket. Book 1 arrives later, so it goes in front, and bucket 2 holds both. Every insert goes to the head of its bucket, so inserting never walks a chain.

The linked list uses the same head insertion. Each new book goes to the front, so the newest book is always first:

![The linked list grows at the head](docs/assets/diagrams/list-head-insert.png)

## Testing

The tests are plain checks, with no framework, and they compare real values. They cover the book constructor, list order and search, the reader's record limit, the number and word readers, the hash range, and the map counts. There are 36 of them, and `make test` runs them.

The more interesting question is whether the tests would notice a bug. To find out, I put ten bugs back into a scratch copy, one at a time. Seven of them fail a plain check straight away. The other three are memory bugs (a read one byte past a string, and two memory leaks), and only the sanitizers see them. So `make test-asan` runs the same checks under AddressSanitizer and UBSan, and it is clean.

A GitHub Actions workflow (`.github/workflows/build.yml`) is set up to run the build, the tests and the sanitizer run on every push and pull request.

## What works, and what is next

**Working today:**
- The linked list: insert, show, and search by number, title and author.
- The hash map: insert, show every bucket, and the counts.
- Both file readers.

**Next, and I am building it:**
- Search in the hash map. Its `recherche_par_*` functions still return `NULL`, so in map mode the search options print nothing. This is the piece that makes the comparison possible.
- The timing comparison that the project is about: measuring how long each lookup takes in the list and in the map, for catalogues of growing size. The repo does not have it yet.
- Adding a new book from the menu (option `6`).

**A known limit:** the search keys in options `3` to `5` are fixed in `main.c`. You can't type your own yet.

## Quick start

You need a C compiler (`gcc` or `clang`) and `make`.

```sh
make                              # builds ./MiniProjet
make test                         # 36 checks, then a smoke run of each backend
make test-asan                    # the same checks under AddressSanitizer and UBSan

./MiniProjet GdeBiblio.txt 1 100  # linked list, first 100 books
./MiniProjet GdeBiblio.txt 2 100  # hash map, first 100 books
```

The three arguments are the catalogue file, the structure (`1` for the linked list, `2` for the hash map), and how many books to read.

| Menu option | What it does |
|---|---|
| `1` | Read *n* books from the file into the structure |
| `2` | Show the list, or every bucket of the map |
| `3` | Search by number (fixed key `4`) |
| `4` | Search by title (fixed key `"UKWAGM"`) |
| `5` | Search by author (fixed key `"gspqoqm"`) |
| `0` | Quit |

## The data

Each line is one book: `number title author`, with single-word titles and authors of at most 29 characters. `GdeBiblio.txt` holds 100,000 books with randomly generated titles and authors. It is made-up data, so the numbers in this README describe how the code behaves, not a real collection.

## Project layout

```
src/
  main.c                 the menu and the program entry point
  structs.[ch]           the book record and its constructor
  LinkedList_biblio.[ch] the linked list and its reader
  Hashmap_biblio.[ch]    the hash map, its hash function and its reader
  entree_sortie.[ch]     helpers that read a file word by word
tests/test_biblio.c      the unit tests
docs/assets/diagrams/    the Excalidraw sources and PNGs used here
Mini_Projet.pdf          the course assignment brief
GdeBiblio.txt            the sample catalogue
```
