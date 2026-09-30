#include "flood_fill.h"
#include <stdint.h>
#include <stdlib.h>
int largest_component(unsigned char *cells, size_t rows, size_t columns, size_t *largest) {
    size_t count, *stack;
    if (!cells || !largest || !rows || !columns || rows > SIZE_MAX / columns) return 0;
    count = rows * columns;
    if (count > SIZE_MAX / sizeof(*stack)) return 0;
    for (size_t i = 0; i < count; ++i) if (cells[i] > 1) return 0;
    stack = malloc(count * sizeof(*stack));
    if (!stack) return 0;
    *largest = 0;
    for (size_t start = 0; start < count; ++start) {
        size_t pending = 0, area = 0;
        if (cells[start] != 1) continue;
        cells[start] = 2;
        stack[pending++] = start;
        while (pending) {
            size_t cell = stack[--pending], row = cell / columns, column = cell % columns;
            size_t neighbours[4], length = 0;
            ++area;
            if (row) neighbours[length++] = cell - columns;
            if (row + 1 < rows) neighbours[length++] = cell + columns;
            if (column) neighbours[length++] = cell - 1;
            if (column + 1 < columns) neighbours[length++] = cell + 1;
            for (size_t i = 0; i < length; ++i)
                if (cells[neighbours[i]] == 1) {
                    cells[neighbours[i]] = 2; /* Mark before pushing: each cell enters once. */
                    stack[pending++] = neighbours[i];
                }
        }
        if (area > *largest) *largest = area;
    }
    free(stack);
    return 1;
}
