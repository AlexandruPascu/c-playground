#include "../algorithms/flood_fill.h"
#include "../common/input.h"
#include <string.h>
#ifndef PLAYGROUND_GRID_FILE
#define PLAYGROUND_GRID_FILE "challenge4.txt"
#endif
int pg_components(int argc, char **argv) {
    FILE *input;
    unsigned char *cells;
    int rows, columns, value;
    size_t count, largest;
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: components [grid.txt]\nInput: rows columns, then binary cells (0 or 1).\n"
             "Finds the largest four-neighbour component. Limit: 1000000 cells.");
        return 0;
    }
    if (argc > 2) { fputs("Use components [grid.txt].\n", stderr); return 1; }
    input = fopen(argc == 2 ? argv[1] : PLAYGROUND_GRID_FILE, "r");
    if (!input) { fputs("Could not open the grid file.\n", stderr); return 1; }
    if (pg_read_int(input, &rows) != 1 || pg_read_int(input, &columns) != 1 ||
        rows < 1 || columns < 1 || rows > 1000000 || columns > 1000000 ||
        rows > 1000000 / columns) {
        fputs("Invalid grid dimensions (maximum 1000000 cells).\n", stderr);
        fclose(input); return 1;
    }
    count = (size_t)rows * (size_t)columns;
    cells = malloc(count);
    if (!cells) { fclose(input); fputs("Could not allocate the grid.\n", stderr); return 1; }
    for (size_t i = 0; i < count; ++i) {
        if (pg_read_int(input, &value) != 1 || (value != 0 && value != 1)) {
            fputs("Grid must contain exactly the specified number of binary cells.\n", stderr);
            free(cells); fclose(input); return 1;
        }
        cells[i] = (unsigned char)value;
    }
    if (pg_read_int(input, &value) != 0) {
        fputs("Unexpected extra data after the grid.\n", stderr);
        free(cells); fclose(input); return 1;
    }
    fclose(input);
    if (!largest_component(cells, (size_t)rows, (size_t)columns, &largest)) {
        free(cells); fputs("Could not process the grid.\n", stderr); return 1;
    }
    printf("Maximum Area: %zu\n", largest);
    free(cells);
    return 0;
}
