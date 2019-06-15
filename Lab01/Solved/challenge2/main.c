#include <stdio.h>
#include <stdlib.h>

int sum(int n, int *v) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += *(v + i);
    }
    return sum;
}

int main()
{
    int n;
    int *v;

    scanf("%d", &n);

    v = (int*) malloc(n * sizeof(int));
    if (!v) {
        printf("Couldn't alloc!\n");
        exit(-1);
    }

    for (int i = 0; i < n; ++i) {
        scanf("%d", &v[i]);
    }

    printf("Sum is %d.\n", sum(n, v));

    return 0;
}
