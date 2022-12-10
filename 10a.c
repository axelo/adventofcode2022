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

    assert(n_states < STATES_CAP - 1);
    states[++n_states] = x;

    assert(n_states >= 220);

    int sum = 20 * states[20] +
              60 * states[60] +
              100 * states[100] +
              140 * states[140] +
              180 * states[180] +
              220 * states[220];

    printf("%d\n", sum);
    return 0;
}
