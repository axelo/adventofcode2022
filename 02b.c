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
    W = 2,
    MAX_OUTCOME,
} Outcome;

static Move you_from_opponent_outcome[MAX_MOVE][MAX_OUTCOME] = {
    {S, R, P}, // opponent R outcome you L, D, W
    {R, P, S}, // opponent P outcome you L, D, W
    {P, S, R}, // opponent S outcome you L, D, W
};

static Move move_from_char(char c) {
    switch (c) {
    case 'A': return R;
    case 'B': return P;
    case 'C': return S;
    default: assert(0 && "Unexpected Move");
    }
}

static Outcome outcome_from_char(char c) {
    switch (c) {
    case 'X': return L;
    case 'Y': return D;
    case 'Z': return W;
    default: assert(0 && "Unexpected Outcome");
    }
}

int main(void) {
    char input_buffer[INPUT_BUFFER_CAP];

    int64_t total_score = 0;

    while (fgets(input_buffer, INPUT_BUFFER_CAP, stdin)) {
        size_t line_length = strlen(input_buffer);
        assert(line_length > 2 && "Expected at least three characters");

        Move opponent = move_from_char(input_buffer[0]);
        Outcome outcome = outcome_from_char(input_buffer[2]);
        Move you = you_from_opponent_outcome[opponent][outcome];

        // + 1 as we use `Move` 0..2 instead of 1..3.
        // * 3 as we use `Outcome` 0..2 instead of 0, 3, 6.
        total_score += (int64_t)you + 1 + outcome * 3;
    }

    assert(feof(stdin) && "Expected EOF set for stdin");

    printf("%lld\n", total_score);

    return 0;
}
