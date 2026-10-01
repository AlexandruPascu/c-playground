#include "../algorithms/fibonacci.h"
#include "../common/input.h"
#include <string.h>
#include <time.h>
static void measure(const char *name, int (*algorithm)(unsigned int), unsigned int n) {
    clock_t start = clock();
    int value = algorithm(n);
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("%s: F(%u) mod %d = %d (%.6fs)\n", name, n, FIBONACCI_MODULUS, value, elapsed);
}
int pg_fibonacci(int argc, char **argv) {
    int n = 30;
    const char *method = argc == 3 ? argv[2] : "all";
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: fibonacci [n [all|recursive|iterative|matrix]]\n"
             "Default n=30. Recursive is limited to n<=35; iterative to n<=10000000.\n"
             "All mode skips methods above their limit. Values are modulo 666013.");
        return 0;
    }
    if (argc > 3 || (argc >= 2 && (!pg_parse_int(argv[1], &n) || n < 0)) ||
        (strcmp(method, "all") && strcmp(method, "recursive") &&
         strcmp(method, "iterative") && strcmp(method, "matrix"))) {
        fputs("Invalid Fibonacci arguments. Use --help.\n", stderr);
        return 1;
    }
    if (strcmp(method, "all") == 0) {
        if (n <= 35) measure("recursive", fibo_recursive, (unsigned int)n);
        else puts("recursive: skipped (n > 35)");
        if (n <= 10000000) measure("iterative", fibo_iterative, (unsigned int)n);
        else puts("iterative: skipped (n > 10000000)");
        measure("matrix", fibo_logarithmic, (unsigned int)n);
    } else if (strcmp(method, "matrix") == 0) measure("matrix", fibo_logarithmic, (unsigned int)n);
    else if (strcmp(method, "iterative") == 0 && n <= 10000000) measure("iterative", fibo_iterative, (unsigned int)n);
    else if (strcmp(method, "recursive") == 0 && n <= 35) measure("recursive", fibo_recursive, (unsigned int)n);
    else { fputs("Requested method exceeds its demonstration limit. Use matrix.\n", stderr); return 1; }
    return 0;
}
