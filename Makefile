CC ?= gcc
CFLAGS ?= -std=gnu11 -Wall -Wextra -O2
LDFLAGS ?= -lm

EXEC       = MiniProjet
SRC_PATH   = src
BUILD_PATH = build
TEST_PATH  = tests
TEST_EXEC  = $(BUILD_PATH)/test_biblio
DATA_FILE  = GdeBiblio.txt

SOURCES     = $(SRC_PATH)/main.c $(filter-out $(SRC_PATH)/main.c,$(wildcard $(SRC_PATH)/*.c))
OBJECTS     = $(patsubst $(SRC_PATH)/%.c,$(BUILD_PATH)/%.o,$(SOURCES))
LIB_OBJECTS = $(filter-out $(BUILD_PATH)/main.o,$(OBJECTS))

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c | $(BUILD_PATH)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TEST_EXEC): $(TEST_PATH)/test_biblio.c $(LIB_OBJECTS) | $(BUILD_PATH)
	$(CC) $(CFLAGS) -Isrc -o $@ $^ $(LDFLAGS)

$(BUILD_PATH):
	mkdir -p $@

# unit tests, then a smoke run of the program for each backend
test: $(TEST_EXEC) $(EXEC)
	./$(TEST_EXEC)
	printf '1\n2\n0\n' | ./$(EXEC) $(DATA_FILE) 1 100 | grep -q "La liste contient : 100 livres"
	printf '1\n0\n' | ./$(EXEC) $(DATA_FILE) 2 100 > /dev/null

# same tests under AddressSanitizer and UBSan: catches the memory errors the checks cannot see
SAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
test-asan:
	$(MAKE) clean
	$(MAKE) test CFLAGS="-std=gnu11 -Wall -g -O1 $(SAN_FLAGS)" LDFLAGS="-lm $(SAN_FLAGS)"

clean:
	rm -rf $(BUILD_PATH) $(EXEC)

.PHONY: all test test-asan clean
