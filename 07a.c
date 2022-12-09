#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // strtoull
#include <string.h>

#define LINE_CAP 128
#define NODE_CAP 512
#define NAME_CAP 16
#define CHILDREN_CAP 16

#define COMMAND_CD "$ cd "
#define COMMAND_LS "$ ls"
#define LS_OUTPUT_DIR "dir "

typedef struct Node {
    char name[NAME_CAP];
    int size;
    char type;
    int parent;
    int n_children;
    int children_index[CHILDREN_CAP];
} Node;

static int find_total_sizes(int node_index, Node *nodes, int *size_n, int sizes[NODE_CAP]) {
    assert(nodes[node_index].type == 'D');

    int relative_size = 0;
    int size_i = *size_n;
    assert(size_i < NODE_CAP);

    *size_n = ++(*size_n);

    for (int i = 0; i < nodes[node_index].n_children; ++i) {
        int child_i = nodes[node_index].children_index[i];

        relative_size += nodes[child_i].size;

        if (nodes[child_i].type == 'D') {
            relative_size += find_total_sizes(child_i, nodes, size_n, sizes);
        }
    }

    return sizes[size_i] = relative_size;
}

int main(void) {
    char line[LINE_CAP];

    Node nodes[NODE_CAP];
    int n_nodes = 0;
    int cwd_index = -1;

    char *found;

    while (fgets(line, LINE_CAP, stdin)) {
        int line_length = strlen(line);

        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[--line_length] = '\0';
        }

        assert(line_length > 0 && "Expected non empty line");

        if (line[0] == '$') {
            if ((found = strstr(line, COMMAND_CD)) != NULL) {
                char *dir_name = found + sizeof(COMMAND_CD) - 1;

                if (strcmp(dir_name, "/") == 0) {
                    int i = n_nodes++;
                    assert(n_nodes < NODE_CAP && "To many nodes");

                    strncpy(nodes[i].name, dir_name, NAME_CAP);
                    nodes[i].size = 0;
                    nodes[i].type = 'D';
                    nodes[i].n_children = 0;
                    nodes[i].parent = -1;

                    cwd_index = i;
                } else if (strcmp(dir_name, "..") == 0) {
                    cwd_index = nodes[cwd_index].parent;
                    assert(cwd_index >= 0);
                } else {
                    assert(cwd_index >= 0);
                    int found_node_index = -1;

                    for (int i = 0; i < nodes[cwd_index].n_children; ++i) {
                        Node child_node = nodes[nodes[cwd_index].children_index[i]];

                        if (child_node.type == 'D' && strcmp(dir_name, child_node.name) == 0) {
                            found_node_index = nodes[cwd_index].children_index[i];
                            break;
                        }
                    }

                    cwd_index = found_node_index;
                    assert(cwd_index >= 0 && "Missing CD directory");
                }

            } else if (strcmp(line, COMMAND_LS) == 0) {
                assert(cwd_index >= 0);
            } else {
                assert(0 && "Unknown command");
            }
        } else {
            int i = n_nodes++;
            assert(n_nodes < NODE_CAP && "To many nodes");
            nodes[i].parent = cwd_index;
            nodes[i].n_children = 0;

            int child_index = nodes[cwd_index].n_children++;
            assert(child_index < CHILDREN_CAP);
            nodes[cwd_index].children_index[child_index] = i;

            if ((found = strstr(line, LS_OUTPUT_DIR)) != NULL) {
                strncpy(nodes[i].name, found + sizeof(LS_OUTPUT_DIR) - 1, NAME_CAP);
                nodes[i].type = 'D';
                nodes[i].size = 0;
            } else {
                int file_size = strtoull(line, &found, 10);
                assert(file_size > 0 && "Expected file size > 0");
                assert(strlen(found) > 0 && "Expected name after file size");

                strncpy(nodes[i].name, found + 1, NAME_CAP);
                nodes[i].size = file_size;
                nodes[i].type = 'F';
            }
        }
    }

    assert(feof(stdin) != 0 && "Expected EOF to be set");

    int sizes[NODE_CAP];
    int n_sizes = 0;
    find_total_sizes(0, nodes, &n_sizes, sizes);
    int total_size = 0;

    for (int i = 0; i < n_sizes; ++i) {
        if (sizes[i] <= 100000) {
            total_size += sizes[i];
        }
    }

    printf("%d\n", total_size);
    return 0;
}
