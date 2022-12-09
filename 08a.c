#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define GRID_CAP 102

int main(void) {
    int grid[GRID_CAP][GRID_CAP] = {0};

    int n_col = 1;
    int n_row = 1;
    char tree_h;

    while (true) {
        tree_h = fgetc(stdin);

        if (tree_h == '\n') {
            ++n_row;
            assert(n_row < GRID_CAP);

            tree_h = fgetc(stdin);

            if (tree_h == EOF) {
                break;
            } else {
                n_col = 1;
            }
        }

        if (tree_h == EOF) {
            ++n_row; // Count EOF as row if wasn't after a newline.
            break;
        }

        assert(tree_h >= '0' && tree_h <= '9');

        grid[n_row][n_col++] = tree_h - '0';
        assert(n_col < GRID_CAP);
    }

    assert(feof(stdin) != 0 && "Expected EOF to be set");
    assert(n_row == n_col && "Expected square grid");

    int visible_from_outside = 0;

    for (int y = 1; y < n_row; ++y) {
        for (int x = 1; x < n_col; ++x) {
            bool right = true;
            for (int x2 = x + 1; x2 < n_row; ++x2) {
                if (grid[y][x2] >= grid[y][x]) {
                    right = false;
                    break;
                }
            }

            bool left = true;
            for (int x2 = x - 1; x2 > 0; --x2) {
                if (grid[y][x2] >= grid[y][x]) {
                    left = false;
                    break;
                }
            }

            bool down = true;
            for (int y2 = y + 1; y2 < n_row; ++y2) {
                if (grid[y2][x] >= grid[y][x]) {
                    down = false;
                    break;
                }
            }

            bool up = true;
            for (int y2 = y - 1; y2 > 0; --y2) {
                if (grid[y2][x] >= grid[y][x]) {
                    up = false;
                    break;
                }
            }

            if (left || right || up || down) {
                ++visible_from_outside;
            }
        }
    }

    printf("%d\n", visible_from_outside);
    return 0;
}
