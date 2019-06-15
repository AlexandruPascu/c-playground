#include <stdio.h>
#include <stdlib.h>

int sum(int n, int *v) {
    int sum = 0;
    // TODO 3: Calculeaza suma elementelor in sum
    return sum;
}

int main()
{
    int n;
    int *v;

    scanf("%d", &n);
    // TODO 1: Aloca n elemente pentru vectorul v
    v = 0;
    
    // TODO 2: Verifica daca s-a putut aloca dimensiunea pentru vector
    if (0) {
        printf("Couldn't alloc!\n");
        exit(-1);
    }

    for (int i = 0; i < n; ++i) {
        scanf("%d", &v[i]);
    }

    printf("Sum is %d.\n", sum(n, v));

    return 0;
}
