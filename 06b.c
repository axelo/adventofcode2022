#include <assert.h>
#include <limits.h> // UCHAR_MAX
#include <stdio.h>
#include <string.h>

#define LINE_CAP 4096

int main(void) {
    char line[LINE_CAP];

    char *line_start = fgets(line, LINE_CAP, stdin);
    assert(line_start != NULL && "Expected a line");

    int markers[UCHAR_MAX] = {0};
    int line_length = strlen(line);
    int i = 0;
    int n_tries = 1;

    while (i < line_length) {
        int marker_index = (int)line[i];

        if (markers[marker_index] == n_tries) {
            i = n_tries++;
        } else {
            if (++i - n_tries == 14 - 1) {
                break;
            }

            markers[marker_index] = n_tries;
        }
    }

    assert(i != line_length && "Start not found");

    printf("%d\n", i);
    return 0;
}
