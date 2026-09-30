#ifndef PLAYGROUND_FLOOD_FILL_H
#define PLAYGROUND_FLOOD_FILL_H
#include <stddef.h>
/* Four-neighbour components; marks visited 1 cells as 2. Returns 0 on error. */
int largest_component(unsigned char *cells, size_t rows, size_t columns, size_t *largest);
#endif
