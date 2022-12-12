#include <assert.h>
#include <stdbool.h>
#include <stdint.h> // int32_t
#include <stdio.h> // fgetc
#include <stdlib.h> // abs

#define MAP_CAP 200
#define NODES_CAP 300

typedef struct {
    int32_t y;
    int32_t x;
} P;

typedef struct {
    int rows;
    int cols;
} Map_size;

static int h_score(P p, P end) {
    return abs(p.x - end.x) + abs(p.y - end.y);
}

static int compare_cost_desc(int f_score[MAP_CAP][MAP_CAP], const P *a, const P *b) {
    return f_score[b->y][b->x] - f_score[a->y][a->x];
}

static int a_star_search(Map_size map_size, uint8_t map[200][200], P start, P end) {
    int g_score[MAP_CAP][MAP_CAP];
    int f_score[MAP_CAP][MAP_CAP];

    for (int i = 0; i < MAP_CAP; ++i) {
        for (int j = 0; j < MAP_CAP; ++j) {
            f_score[i][j] = INT32_MAX; // "Infinity"
            g_score[i][j] = INT32_MAX; // "Infinity"
        }
    }

    P visited_nodes[NODES_CAP];
    int n_visited_nodes = 0;

    P deltas[] = {
        {.y = 0, .x = -1},
        {.y = 0, .x = 1},
        {.y = -1, .x = 0},
        {.y = 1, .x = 0},
    };
    int n_deltas = sizeof(deltas) / sizeof(deltas[0]);

    visited_nodes[n_visited_nodes++] = start;
    assert(n_visited_nodes < NODES_CAP);

    g_score[start.y][start.x] = 0;
    f_score[start.y][start.x] = h_score(start, end);

    while (n_visited_nodes > 0) {
        // Seems to be fast enough for this problem, and simple to code.
        qsort_r(visited_nodes, n_visited_nodes, sizeof(visited_nodes[0]),
                f_score, (int (*)(void *, const void *, const void *))compare_cost_desc);

        // Last one is the one with the lowest f score thanks to the sort above
        P current = visited_nodes[--n_visited_nodes];

        if (current.y == end.y && current.x == end.x) {
            return f_score[current.y][current.x];
        }

        int current_height = map[current.y][current.x];

        for (int delta_i = 0; delta_i < n_deltas; ++delta_i) {
            P neighbor = {.x = current.x + deltas[delta_i].x,
                          .y = current.y + deltas[delta_i].y};

            if (neighbor.y < 0 || neighbor.x < 0 ||
                neighbor.y >= map_size.rows || neighbor.x >= map_size.cols) {
                continue;
            }

            int neighbor_height = map[neighbor.y][neighbor.x];

            if (neighbor_height - current_height <= 1) {
                int distance = abs(deltas[delta_i].x + deltas[delta_i].y); // Always 1 in this problem, but hey
                int tentative_g_score = g_score[current.y][current.x] + distance;
                int neighbor_g_score = g_score[neighbor.y][neighbor.x];

                if (tentative_g_score < neighbor_g_score) {
                    g_score[neighbor.y][neighbor.x] = tentative_g_score;
                    f_score[neighbor.y][neighbor.x] = tentative_g_score + h_score(neighbor, end);

                    bool never_visited = true;
                    for (int i = 0; i < n_visited_nodes; ++i) {
                        if (visited_nodes[i].x == neighbor.x &&
                            visited_nodes[i].y == neighbor.y) {

                            never_visited = false;
                            break;
                        }
                    }

                    if (never_visited) {
                        visited_nodes[n_visited_nodes++] = neighbor;
                        assert(n_visited_nodes < NODES_CAP);
                    }
                }
            }
        }
    }

    // End not found
    return INT32_MAX;
}

int main(void) {
    uint8_t map[MAP_CAP][MAP_CAP] = {0};

    char input_h = '\0';
    Map_size map_size = {0};
    P end = {0};

    while ((input_h = fgetc(stdin)) != EOF) {
        if (input_h == '\n') {
            input_h = fgetc(stdin);

            if (input_h != EOF) {
                ++map_size.rows;
                assert(map_size.rows < MAP_CAP);
                map_size.cols = 0;
            }
        }

        if (input_h == EOF) {
            ++map_size.rows;
            assert(map_size.rows < MAP_CAP);
            break;
        }

        if (input_h == 'S') {
            input_h = 'a';
        } else if (input_h == 'E') {
            end.y = map_size.rows;
            end.x = map_size.cols;
            input_h = 'z';
        }

        assert(input_h >= 'a' && input_h <= 'z');
        map[map_size.rows][map_size.cols++] = input_h - 'a';
        assert(map_size.cols < MAP_CAP);
    }

    int min_steps = INT32_MAX;

    for (int y = 0; y < map_size.rows; ++y) {
        for (int x = 0; x < map_size.cols; ++x) {
            if (map[y][x] == 0) {
                int steps = a_star_search(map_size, map, (P){.x = x, .y = y}, end);

                if (steps < min_steps) {
                    min_steps = steps;
                }
            }
        }
    }

    assert(min_steps != INT32_MAX);

    printf("%d\n", min_steps);
    return 0;
}
