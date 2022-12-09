#include <assert.h>
#include <stdio.h>
#include <string.h> // strchr

#define LINE_CAP 1024
#define STACK_CAP 16
#define CRATES_CAP 64

typedef enum {
    CRATES = 1,
    END_OF_STACKS,
    MOVES,
} Parse_Mode;

static void reverse_stack_inplace(int n_crates, char stack[n_crates]) {
    for (int i = 0; i < n_crates / 2; ++i) {
        char c = stack[i];
        int j = n_crates - i - 1;
        stack[i] = stack[j];
        stack[j] = c;
    }
}

int main(void) {
    char line[LINE_CAP];
    Parse_Mode parse_mode = CRATES;

    char stacks[STACK_CAP][CRATES_CAP];
    int n_crates[STACK_CAP] = {0};
    int n_stacks = 0;

    while (fgets(line, LINE_CAP, stdin)) {
        switch (parse_mode) {
        case CRATES: {
            char *crate_start = strchr(line, '[');

            if (crate_start == NULL) {
                parse_mode = END_OF_STACKS;

                for (int i = 0; i < n_stacks; ++i) {
                    reverse_stack_inplace(n_crates[i], stacks[i]);
                }
            } else {
                while (crate_start != NULL) {
                    int stack_index = (crate_start - line) / 4;
                    assert(stack_index >= 0 && stack_index < STACK_CAP && "stack_index out of bounds");

                    int crate_index = n_crates[stack_index]++;
                    assert(crate_index >= 0 && crate_index < CRATES_CAP && "crate_index out of bounds");

                    const char crate = *(crate_start + 1);

                    stacks[stack_index][crate_index] = crate;

                    if (stack_index + 1 > n_stacks) {
                        n_stacks = stack_index + 1;
                    }

                    crate_start = strchr(crate_start + 1, '[');
                }
            }

        } break;
        case END_OF_STACKS:
            if (line[0] == '\n') {
                parse_mode = MOVES;
            }
            break;
        case MOVES: {
            int amount = 0;
            int from_stack = 0;
            int to_stack = 0;

            int found = sscanf(line, "move %d from %d to %d", &amount, &from_stack, &to_stack, NULL);
            assert(found == 3 && "Unexpected move syntax");

            for (int i = 0; i < amount; ++i) {
                int from_stack_crate_end_index = --n_crates[from_stack - 1];
                assert(from_stack_crate_end_index >= 0 && "from_stack_crate_end_index out of bounds");

                char crate = stacks[from_stack - 1][from_stack_crate_end_index];

                int to_stack_crate_end_index = n_crates[to_stack - 1]++;
                assert(to_stack_crate_end_index < CRATES_CAP && "to_stack_crate_end_index out of bounds");

                stacks[to_stack - 1][to_stack_crate_end_index] = crate;
            }
        } break;
        }
    }

    assert(feof(stdin) != 0 && "Expected EOF to be set");

    for (int i = 0; i < n_stacks; ++i) {
        putc(stacks[i][n_crates[i] - 1], stdout);
    }
    puts("");

    return 0;
}
