/* Completed introductory exercises; shared by the original and Solved entry points. */
#include "apps.h"
#include "list.h"
#include "../common/input.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
static int usage(int argc, char **argv, const char *message) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) { puts(message); return 1; }
    if (argc != 1) { fputs(message, stderr); fputc('\n', stderr); return -1; }
    return 0;
}
static int read_real(FILE *input, double *value) {
    char token[128], *end;
    int status = pg_read_token(input, token, sizeof(token));
    if (status != 1) return status;
    errno = 0;
    *value = strtod(token, &end);
    return !*token || *end || errno == ERANGE || !isfinite(*value) ? -1 : 1;
}
int pg_numbers(int argc, char **argv) {
    double value, integer;
    int status = usage(argc, argv, "Usage: numbers\nRead finite numbers until a negative value or EOF. Integers go to stdout, fractions to stderr.");
    if (status) return status < 0;
    while ((status = read_real(stdin, &value)) == 1) {
        if (value < 0) return 0;
        if (modf(value, &integer) == 0) printf("%.0f\n", value);
        else fprintf(stderr, "%.2f\n", value);
    }
    if (status < 0) fputs("Invalid number.\n", stderr);
    return status < 0;
}
int pg_vector_sum(int argc, char **argv) {
    int n, *values, status = usage(argc, argv, "Usage: vector_sum\nInput: count (0..1000000), then that many integers.");
    long long total = 0;
    if (status) return status < 0;
    if (pg_read_int(stdin, &n) != 1 || n < 0 || n > 1000000) { fputs("Invalid vector size.\n", stderr); return 1; }
    values = n ? malloc((size_t)n * sizeof(*values)) : NULL;
    if (n && !values) { fputs("Could not allocate the vector.\n", stderr); return 1; }
    for (int i = 0; i < n; ++i) {
        if (pg_read_int(stdin, &values[i]) != 1) { free(values); fputs("Invalid vector element.\n", stderr); return 1; }
    }
    for (int i = 0; i < n; ++i) total += values[i];
    printf("Sum is %lld.\n", total);
    free(values);
    return 0;
}
static double snap_second(double time) {
    double nearest = floor(time + 0.5);
    return fabs(time - nearest) < 1e-9 ? nearest : time;
}
int pg_fps(int argc, char **argv) {
    int n, *buckets, seconds;
    double *ends, total = 0, previous = 0;
    long long sum = 0;
    int status = usage(argc, argv, "Usage: fps\nInput: frame count (0..1000000), then frame durations of at least 0.000001 seconds.\nCounts frames overlapping each second; maximum total duration 1000000 seconds.");
    if (status) return status < 0;
    if (pg_read_int(stdin, &n) != 1 || n < 0 || n > 1000000) { fputs("Invalid frame count.\n", stderr); return 1; }
    ends = n ? malloc((size_t)n * sizeof(*ends)) : NULL;
    if (n && !ends) return 1;
    for (int i = 0; i < n; ++i) {
        double duration;
        if (read_real(stdin, &duration) != 1 || duration < 0.000001 || total + duration > 1000000 || total + duration == total) {
            free(ends); fputs("Invalid frame duration or total duration too large.\n", stderr); return 1;
        }
        total += duration;
        ends[i] = total;
    }
    seconds = (int)ceil(snap_second(total));
    buckets = seconds ? calloc((size_t)seconds, sizeof(*buckets)) : NULL;
    if (seconds && !buckets) { free(ends); return 1; }
    for (int i = 0; i < n; ++i) {
        int first = (int)floor(snap_second(previous));
        int last = (int)ceil(snap_second(ends[i]));
        for (int second = first; second < last; ++second) ++buckets[second];
        previous = ends[i];
    }
    printf("Fps array:");
    for (int i = 0; i < seconds; ++i) { printf(" %d", buckets[i]); sum += buckets[i]; }
    printf("\nAverage fps: %lld\n", seconds ? sum / seconds : 0);
    free(ends); free(buckets);
    return 0;
}
int pg_highlight(int argc, char **argv) {
    char text[102], copy[102], word[12];
    size_t length, word_length;
    int status = usage(argc, argv, "Usage: highlight\nInput: a line of at most 100 characters, then a search word of at most 10 characters.");
    if (status) return status < 0;
    if (!fgets(text, sizeof(text), stdin)) { fputs("Missing text.\n", stderr); return 1; }
    length = strcspn(text, "\r\n");
    if (length > 100 || pg_read_token(stdin, word, sizeof(word)) != 1 || strlen(word) > 10) {
        fputs("Text or word is too long, or missing.\n", stderr); return 1;
    }
    text[length] = '\0';
    for (size_t i = 0; i <= length; ++i) copy[i] = (char)tolower((unsigned char)text[i]);
    word_length = strlen(word);
    for (size_t i = 0; i < word_length; ++i) word[i] = (char)tolower((unsigned char)word[i]);
    /* Advance by one, so overlapping occurrences are highlighted too. */
    for (size_t i = 0; i + word_length <= length; ++i)
        if (memcmp(copy + i, word, word_length) == 0)
            for (size_t j = 0; j < word_length; ++j) text[i + j] = (char)toupper((unsigned char)text[i + j]);
    puts(text);
    return 0;
}
static void print_list(const PgList *list) {
    int first = 1;
    for (const PgNode *node = list->head; node; node = node->next) {
        printf("%s%d", first ? "" : " ", node->value); first = 0;
    }
    putchar('\n');
}
int pg_list(int argc, char **argv) {
    PgList list = {0};
    char command[32];
    int status = usage(argc, argv, "Usage: list_ops\nCommands: push_front N, push_back N, pop_front, pop_back, find N, min, max, print, quit.");
    if (status) return status < 0;
    puts("List commands: push_front N, push_back N, pop_front, pop_back, find N, min, max, print, quit.");
    while ((status = pg_read_token(stdin, command, sizeof(command))) == 1) {
        int value, minimum, maximum;
        if (!strcmp(command, "quit")) break;
        if (!strcmp(command, "push_front") || !strcmp(command, "push_back") || !strcmp(command, "find")) {
            if (pg_read_int(stdin, &value) != 1) { status = -1; break; }
            if (!strcmp(command, "find")) puts(pg_list_contains(&list, value) ? "found" : "not found");
            else if (list.size >= 1000000 || !pg_list_push(&list, value, !strcmp(command, "push_front"))) { status = -1; break; }
        } else if (!strcmp(command, "pop_front") || !strcmp(command, "pop_back")) {
            if (pg_list_pop(&list, &value, !strcmp(command, "pop_front"))) printf("%d\n", value);
            else puts("empty");
        } else if (!strcmp(command, "min") || !strcmp(command, "max")) {
            if (pg_list_extrema(&list, &minimum, &maximum)) printf("%d\n", !strcmp(command, "min") ? minimum : maximum);
            else puts("empty");
        } else if (!strcmp(command, "print")) print_list(&list);
        else { status = -1; break; }
        fflush(stdout);
    }
    pg_list_clear(&list);
    if (status < 0) fputs("Invalid command, missing integer, or allocation/size limit reached.\n", stderr);
    return status < 0;
}
int pg_sort(int argc, char **argv) {
    PgList list = {0};
    int count, value, status = usage(argc, argv, "Usage: insertion_sort\nInput: count (0..10000), then nonnegative integers. Inserts each number directly into a sorted list.");
    if (status) return status < 0;
    if (pg_read_int(stdin, &count) != 1 || count < 0 || count > 10000) { fputs("Invalid element count.\n", stderr); return 1; }
    for (int i = 0; i < count; ++i) {
        if (pg_read_int(stdin, &value) != 1 || value < 0 || !pg_list_insert_sorted(&list, value)) {
            pg_list_clear(&list); fputs("Invalid element or allocation failure.\n", stderr); return 1;
        }
    }
    print_list(&list);
    pg_list_clear(&list);
    return 0;
}
