#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // strtoull

#define LINE_CAP 16
#define GRID_CAP 700
#define N_KNOTS 10

int main(void) {
    char line[LINE_CAP];
    int grid[GRID_CAP][GRID_CAP] = {0};

    int knot_x[N_KNOTS];
    int knot_y[N_KNOTS];

    for (int i = 0; i < N_KNOTS; ++i) {
        knot_y[i] = GRID_CAP / 2;
        knot_x[i] = GRID_CAP / 2;
    }

    int h_i = 0;
    int t_i = N_KNOTS - 1;

    grid[knot_y[t_i]][knot_x[t_i]] = 1;

    while (fgets(line, LINE_CAP, stdin)) {
        char dir = line[0];
        int steps = strtoul(line + 2, NULL, 10);
        int dir_dx = dir == 'R'   ? 1
                     : dir == 'L' ? -1
                                  : 0;
        int dir_dy = dir == 'D'   ? 1
                     : dir == 'U' ? -1
                                  : 0;

        for (int step = 0; step < steps; ++step) {
            knot_y[h_i] += dir_dy;
            knot_x[h_i] += dir_dx;

            for (int i = 1; i < N_KNOTS; ++i) {
                int k_dy = knot_y[i] < knot_y[i - 1]   ? 1
                           : knot_y[i] > knot_y[i - 1] ? -1
                                                       : 0;
                int k_dx = knot_x[i] < knot_x[i - 1]   ? 1
                           : knot_x[i] > knot_x[i - 1] ? -1
                                                       : 0;
                int next_ky = knot_y[i] + k_dy;
                int next_kx = knot_x[i] + k_dx;

                if (next_kx != knot_x[i - 1] || next_ky != knot_y[i - 1]) {
                    knot_y[i] = next_ky;
                    knot_x[i] = next_kx;

                    assert(next_ky >= 0 && next_ky < GRID_CAP &&
                           next_kx >= 0 && next_kx < GRID_CAP);

                    if (i == t_i) {
                        ++grid[next_ky][next_kx];
                    }
                }
            }
        }
    }

    assert(feof(stdin) != 0);

    int tail_visit_at_least_once = 0;

    for (int y = 0; y < GRID_CAP; ++y) {
        for (int x = 0; x < GRID_CAP; ++x) {
            if (grid[y][x] > 0) {
                ++tail_visit_at_least_once;
            }
        }
    }

    printf("%d\n", tail_visit_at_least_once);
}
