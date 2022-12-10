#include <assert.h>
#include <stdio.h> // fgets
#include <stdlib.h> // strtol
#include <string.h> // strstr

#define LINE_CAP 32
#define STATES_CAP 300

int main(void) {
    char line[LINE_CAP];

    int states[STATES_CAP];
    int n_states = 0;
    int x = 1;

    states[0] = x;

    while (fgets(line, LINE_CAP, stdin)) {
        if (strstr(line, "noop") != NULL) {
            assert(n_states < STATES_CAP - 1);
            states[++n_states] = x;
        } else if (strstr(line, "addx") != NULL) {
            int imm = strtol(line + sizeof("addx"), NULL, 10);

            assert(n_states < STATES_CAP - 1);
            states[++n_states] = x;

            assert(n_states < STATES_CAP - 1);
            states[++n_states] = x;

            x += imm;
        } else {
            assert(0 && "Unknown instruction");
        }
    }

    assert(feof(stdin) != 0);

    assert(n_states >= 220);

    int crt_row = 0;

    for (int c = 1; c <= n_states; ++c) {
        int crt_curr_row = (c - 1) / 40;
        int crt_col = (c - 1) % 40;

        if (crt_curr_row != crt_row) {
            puts("");
            crt_row = crt_curr_row;
            assert(crt_row < 6);
        }

        int sprite_x = states[c];

        if (abs(sprite_x - crt_col) < 2) {
            printf("\u2588");
        } else {
            putc(' ', stdout);
        }
    }
    puts("");
    return 0;
}
