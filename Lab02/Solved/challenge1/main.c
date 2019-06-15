#include <stdio.h>
#include <time.h>
#define fibo_small 40
#define fibo_huge 400000000
#define mod 666013
int fibo_recursive(int n) {
    if(n == 1 || n == 2)
        return 1;
    return (fibo_recursive(n - 1) % mod + fibo_recursive(n - 2) % mod) % mod;
}
int fibo_iterative(int n) {
    int first = 1;
    int second = 1;
    int result;
    for (int i = 3; i <= n; i++) {
        result = (first + second) % mod;
        first = second;
        second = result;
    }
    return result;
}
void multiply(int a[2][2], int b[2][2]) {
    int result[2][2];
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            result[i][j] = 0;
            for (int p = 0; p < 2; ++p)
                result[i][j] += (1LL * a[i][p] * b[p][j]) % mod;
        }
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            a[i][j] = result[i][j] % mod;
}
int fibo_logarithmic(int n) {
    int result[2][2], matrix[2][2];
    result[1][1] = 1;
    result[0][0] = result[0][1] = result[1][0] = 0;
    matrix[0][0] = 0;
    matrix[0][1] = matrix[1][0] = matrix[1][1] = 1;

    --n;
    while (n) {
        if (n % 2 == 1) {
            --n;
            multiply(result, matrix);
        }
        else {
            n /= 2;
            multiply(matrix, matrix);
        }
    }
    return result[1][1];
}
double measure_time(int (*fibo)(), int n) {
    clock_t start = clock();
    fibo(n);
    clock_t end = clock();
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}
int main() {
    printf("Al %d - lea termen Fibonacci este %d\n", fibo_small, fibo_logarithmic(fibo_small));
    printf("%lfs - Recursive\n", measure_time(fibo_recursive, fibo_small));
    printf("%lfs - Iterative\n", measure_time(fibo_iterative, fibo_small));
    printf("%lfs - Logarithmic\n", measure_time(fibo_logarithmic, fibo_small));

    printf("Al %d - lea termen Fibonacci este %d\n", fibo_huge, fibo_logarithmic(fibo_huge));
    printf("%lfs - Iterative\n", measure_time(fibo_iterative, fibo_huge));
    printf("%lfs - Logarithmic\n", measure_time(fibo_logarithmic, fibo_huge));

    return 0;
}
