/* Recurrence and matrix approach adapted from Lab02's reference solution. */
#include "fibonacci.h"
#include <stdint.h>
int fibo_recursive(unsigned int n) {
    if (n < 2) return (int)n;
    return (fibo_recursive(n - 1) + fibo_recursive(n - 2)) % FIBONACCI_MODULUS;
}
int fibo_iterative(unsigned int n) {
    int previous = 0, current = 1;
    for (unsigned int i = 0; i < n; ++i) {
        int next = (previous + current) % FIBONACCI_MODULUS;
        previous = current;
        current = next;
    }
    return previous;
}
static void multiply(int a[2][2], int b[2][2]) {
    int result[2][2];
    for (int row = 0; row < 2; ++row)
        for (int column = 0; column < 2; ++column) {
            uint64_t sum = 0;
            for (int k = 0; k < 2; ++k)
                sum += (uint64_t)a[row][k] * (uint64_t)b[k][column];
            result[row][column] = (int)(sum % FIBONACCI_MODULUS);
        }
    for (int row = 0; row < 2; ++row)
        for (int column = 0; column < 2; ++column)
            a[row][column] = result[row][column];
}
int fibo_logarithmic(unsigned int n) {
    int result[2][2] = {{1, 0}, {0, 1}};
    int matrix[2][2] = {{1, 1}, {1, 0}};
    while (n) {
        if (n & 1U) multiply(result, matrix);
        n >>= 1;
        if (n) multiply(matrix, matrix);
    }
    return result[0][1];
}
