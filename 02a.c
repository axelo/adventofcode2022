#include <assert.h> // assert
#include <stdint.h> // int64_t
#include <stdio.h> // printf, fgetc
#include <string.h> // strlen

#define INPUT_BUFFER_CAP (32 * 1024)

typedef enum {
    R = 0,
    P = 1,
    S = 2,
    MAX_MOVE,
} Move;

typedef enum {
    L = 0,
    D = 1,
    W = 2
} Outcome;

static Outcome opponent_vs_you[MAX_MOVE][MAX_MOVE] = {
    {D, L, W}, // you R vs R, P, S
    {W, D, L}, // you P vs R, P, S
    {L, W, D}, // you S vs R, P, S
};

static Move move_from_char(char c) {
    switch (c) {
    case 'A':
    case 'X': return R;

    case 'B':
    case 'Y': return P;

    case 'C':
    case 'Z': return S;

    default: assert(0 && "Unexpected move");
    }
}

int main(void) {
    char input_buffer[INPUT_BUFFER_CAP];

    int64_t total_score = 0;

    while (fgets(input_buffer, INPUT_BUFFER_CAP, stdin)) {
        size_t line_length = strlen(input_buffer);
        assert(line_length > 2 && "Expected at least three characters");

        Move opponent = move_from_char(input_buffer[0]);
        Move you = move_from_char(input_buffer[2]);

        // +1 as we use `Move` 0..2 instead of 1..3.
        total_score += (int64_t)you + 1 + (int64_t)opponent_vs_you[you][opponent] * 3;
    }

    assert(feof(stdin) && "Expected EOF set for stdin");

    printf("%lld\n", total_score);

    return 0;
}
