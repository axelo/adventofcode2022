#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // stotol
#include <string.h>

#define LINE_CAP 64
#define M_CAP 8
#define ITEMS_CAP 350

typedef struct {
    char op;
    int op_imm;
    int test_div_by;
    int m_when_true;
    int m_when_false;
    int n_items;
    int items[ITEMS_CAP];
    int inspected_items;
} M;

static int compare_most_inspected_items(M *a, M *b) {
    return b->inspected_items - a->inspected_items;
}

int main(void) {
    char line[LINE_CAP];

    M ms[M_CAP] = {0};
    int n_m = 0;
    int m_i = -1;

    while (fgets(line, LINE_CAP, stdin)) {
        if (strstr(line, "Monkey ")) {
            int m = strtoul(line + sizeof("Monkey"), NULL, 10);
            assert(m < M_CAP);

            ++n_m;
            assert(n_m <= M_CAP);

            m_i = m;
        } else if (strstr(line, "  Starting items: ")) {
            char *lasts;

            for (char *token = strtok_r(line + sizeof("  Starting items:"), ", ", &lasts);
                 token != NULL;
                 token = strtok_r(NULL, ", ", &lasts)) {

                int item_i = ms[m_i].n_items++;
                assert(ms[m_i].n_items < ITEMS_CAP);

                ms[m_i].items[item_i] = strtoul(token, NULL, 10);
            }
        } else if (strstr(line, "  Operation: new = old ")) {
            char op = line[sizeof("  Operation: new = old")];
            assert(op == '*' || op == '+');

            if (strstr(line + sizeof("  Operation: new = old"), "* old")) {
                ms[m_i].op = '^';
                ms[m_i].op_imm = 2;
            } else {
                ms[m_i].op = op;
                ms[m_i].op_imm = strtoul(line + sizeof("  Operation: new = old ?"), NULL, 10);
            }
        } else if (strstr(line, "  Test: divisible by ")) {
            int test_div_by = strtoul(line + sizeof("  Test: divisible by"), NULL, 10);
            assert(test_div_by > 0);

            ms[m_i].test_div_by = test_div_by;
        } else if (strstr(line, "    If true: throw to monkey ")) {
            int m = strtoul(line + sizeof("    If true: throw to monkey"), NULL, 10);
            assert(m < M_CAP);

            ms[m_i].m_when_true = m;
        } else if (strstr(line, "    If false: throw to monkey ")) {
            int m = strtoul(line + sizeof("    If false: throw to monkey"), NULL, 10);
            assert(m < M_CAP);

            ms[m_i].m_when_false = m;
        }
    }

    assert(feof(stdin) != 0);
    assert(n_m >= 2);

    for (int r = 0; r < 20; ++r) {
        for (int i = 0; i < n_m; ++i) {
            for (int j = 0; j < ms[i].n_items; ++j) {
                int item = ms[i].items[j];

                if (item == INT_MIN) { // item "removed" from m.
                    continue;
                }

                char op = ms[i].op;

                if (op == '*') {
                    item *= ms[i].op_imm;
                } else if (op == '+') {
                    item += ms[i].op_imm;
                } else if (op == '^') {
                    assert(ms[i].op_imm == 2);
                    item *= item;
                } else {
                    assert(false && "Unknown operation");
                }

                item /= 3;

                bool is_divisible = (item % ms[i].test_div_by) == 0;
                int dest_m_i = is_divisible ? ms[i].m_when_true : ms[i].m_when_false;

                int dest_m_item_i = ms[dest_m_i].n_items++;
                assert(ms[dest_m_i].n_items < ITEMS_CAP);

                ms[dest_m_i].items[dest_m_item_i] = item;

                ++ms[i].inspected_items;

                ms[i].items[j] = INT_MIN;
            }
        }
    }

    qsort(ms, n_m, sizeof(ms[0]), (int (*)(const void *, const void *))compare_most_inspected_items);

    int m_business = ms[0].inspected_items * ms[1].inspected_items;

    printf("%d\n", m_business);
    return 0;
}
