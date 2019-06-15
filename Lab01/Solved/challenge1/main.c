#include <stdio.h>

int main()
{
    float x;
    scanf("%f", &x);

    while (x >= 0) {
        if (x - (int)x == 0) {
            fprintf(stdout, "%d ", (int)x);
        } else {
            fprintf(stderr, "%.2f ", x);
        }
        scanf("%f", &x);
    }

    return 0;
}
