#include <assert.h>
#include <stdio.h>

#define GRID_CAP 102

int main(void) {
    int grid[GRID_CAP][GRID_CAP] = {0};

    int n_col = 1;
    int n_row = 1;
    char tree_h;

    while (1) {
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

    int highest_scenic_score = 0;

    for (int y = 1; y < n_row; ++y) {
        for (int x = 1; x < n_col; ++x) {
            int right = 0;
            for (int x2 = x + 1; x2 < n_row; ++x2) {
                ++right;
                if (grid[y][x2] >= grid[y][x]) {
                    break;
                }
            }

            int left = 0;
            for (int x2 = x - 1; x2 > 0; --x2) {
                ++left;
                if (grid[y][x2] >= grid[y][x]) {
                    break;
                }
            }

            int down = 0;
            for (int y2 = y + 1; y2 < n_row; ++y2) {
                ++down;
                if (grid[y2][x] >= grid[y][x]) {
                    break;
                }
            }

            int up = 0;
            for (int y2 = y - 1; y2 > 0; --y2) {
                ++up;
                if (grid[y2][x] >= grid[y][x]) {
                    break;
                }
            }

            int scenic_score = right * left * down * up;

            if (scenic_score > highest_scenic_score) {
                highest_scenic_score = scenic_score;
            }
        }
    }

    printf("%d\n", highest_scenic_score);
    return 0;
}
