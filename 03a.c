#include <assert.h> // assert
#include <limits.h> // UCHAR_MAX
#include <stdio.h> // printf
#include <string.h> // strlen

#define INPUT_BUFFER_CAP (8 * 1024)

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

    while (fgets(input_buffer, INPUT_BUFFER_CAP, stdin)) {
        int line_length = strlen(input_buffer);
        assert(line_length > 0 && "Expected non empty line length");

        int compartment_size = line_length / 2; // We don't care about new line character.
        int count_items_by_type_compartment_one[UCHAR_MAX + 1] = {0};
        int count_items_by_type_compartment_two[UCHAR_MAX + 1] = {0};

        for (int i = 0; i < compartment_size; ++i) {
            ++count_items_by_type_compartment_one[(int)input_buffer[i]];
            ++count_items_by_type_compartment_two[(int)input_buffer[i + compartment_size]];
        }

        for (int item_type = 0; item_type <= UCHAR_MAX; ++item_type) {
            int count_compartment_one = count_items_by_type_compartment_one[item_type];
            int count_compartment_two = count_items_by_type_compartment_two[item_type];

            if (count_compartment_one > 0 && count_compartment_two > 0) {
                sum_priority += prioritize_from_char(item_type);
            }
        }
    }

    printf("%d\n", sum_priority);

    return 0;
}
