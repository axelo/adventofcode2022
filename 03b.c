#include <assert.h> // assert
#include <limits.h> // UCHAR_MAX
#include <stdio.h> // printf
#include <string.h> // strlen

#define INPUT_BUFFER_CAP (8 * 1024)
#define GROUP_CAP (3)

static inline int prioritize_from_char(char c) {
    return c >= 'a' && c <= 'z'
               ? c - 'a' + 1
           : c >= 'A' && c <= 'Z'
               ? c - 'A' + 27
               : 0;
}

int main(void) {
    char input_buffer[INPUT_BUFFER_CAP];

    int sum_priority = 0;

    int count_items_by_type[GROUP_CAP][UCHAR_MAX + 1] = {0};
    int group_index = 0;

    while (fgets(input_buffer, INPUT_BUFFER_CAP, stdin)) {
        int line_length = strlen(input_buffer);
        assert(line_length > 0 && "Expected non empty line length");

        for (int i = 0; i < line_length; ++i) {
            ++count_items_by_type[group_index][(int)input_buffer[i]];
        }

        if (++group_index == GROUP_CAP) {
            group_index = 0;

            for (int i = 0; i < UCHAR_MAX + 1; ++i) {
                int count_sum = 0;

                for (int j = 0; j < GROUP_CAP; ++j) {
                    count_sum += count_items_by_type[j][i] ? 1 : 0;
                    count_items_by_type[j][i] = 0; // Clear count for next group.
                }

                assert(count_sum <= GROUP_CAP && "count_sum greater than GROUP_CAP");

                if (count_sum == GROUP_CAP) {
                    sum_priority += prioritize_from_char(i);
                }
            }
        }
    }

    printf("%d\n", sum_priority);

    return 0;
}
