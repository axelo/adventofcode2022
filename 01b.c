#include <assert.h> // assert
#include <stdio.h> // printf, fgetc
#include <stdlib.h> // strtoull, qsort

#define ELF_CAP 300
#define INPUT_CAP 32 * 1024

static int parse_from_stdin(int n, uint64_t elf_calories[n]) {
    static char parse_input_buffer[INPUT_CAP];

    uint64_t n_elves = 0;
    uint64_t current_calories = 0;

    while (fgets(parse_input_buffer, INPUT_CAP, stdin)) {
        uint64_t calories = strtoull(parse_input_buffer, NULL, 10);

        if (calories == 0) {
            elf_calories[n_elves++] = current_calories;
            current_calories = 0;

            assert(n_elves < n && "Too many elves");
        } else {
            current_calories += calories;
        }
    }

    if (current_calories > 0) {
        elf_calories[n_elves++] = current_calories;
    }

    assert(feof(stdin) != 0 && "Unexpected EOF error");
    assert(ferror(stdin) == 0 && "Unexpected file error");

    return n_elves;
}

static int greater_than(const int64_t *a, const int64_t *b) {
    return *b - *a;
}

int main(void) {
    static uint64_t elf_calories[ELF_CAP];

    int n_elves = parse_from_stdin(ELF_CAP, elf_calories);

    assert(n_elves > 2 && "Expected at least three elves");

    qsort(elf_calories, n_elves, sizeof(uint64_t), (int (*)(const void *, const void *))greater_than);

    printf("%llu\n", elf_calories[0] + elf_calories[1] + elf_calories[2]);

    return 0;
}
