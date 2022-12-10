#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // strtoull

#define LINE_CAP 16
#define GRID_CAP 700

int main(void) {
    char line[LINE_CAP];
    int grid[GRID_CAP][GRID_CAP] = {0};

    int hx = GRID_CAP / 2;
    int hy = GRID_CAP / 2;
    int tx = hx;
    int ty = hy;

    grid[ty][tx] = 1;

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
            hx += dir_dx;
            hy += dir_dy;

            int t_dx = tx < hx   ? 1
                       : tx > hx ? -1
                                 : 0;
            int t_dy = ty < hy   ? 1
                       : ty > hy ? -1
                                 : 0;

            int next_tx = tx + t_dx;
            int next_ty = ty + t_dy;

            if (next_tx != hx || next_ty != hy) {
                tx = next_tx;
                ty = next_ty;

                assert(tx >= 0 && tx < GRID_CAP && ty >= 0 && ty < GRID_CAP);

                ++grid[ty][tx];
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
