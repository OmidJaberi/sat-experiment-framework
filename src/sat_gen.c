#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define K 3
#define MAXN 1000
#define MAXALPHA 20

typedef struct {
    int literal[K];
    bool is_inverted[K];
} clause_t;

typedef struct {
    int n, m;
    double alpha;
    clause_t list[MAXN * MAXALPHA];
} cnf_t;

static void _random_shuffle(int *a, int n, int k)
{
    for (int i = 0; i < n; i++)
        a[i] = i + 1;
    for (int i = 0; i < k; i++)
    {
        int j = i + rand() % (n - i);
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void cnf_gen(cnf_t *formula, int n, double alpha)
{
    formula->n = n;
    formula->m = (int)round(n * alpha); 
    formula->alpha = alpha;
    for (int i = 0; i < formula->m; i++)
    {
        int arr[MAXN];
        _random_shuffle(arr, n, K);
        for (int j = 0; j < K; j++)
        {
            formula->list[i].literal[j] = arr[j];
            formula->list[i].is_inverted[j] = rand() % 2;
        }
    }
}

int cnf_to_dimacs(cnf_t formula, const char *fname)
{
    FILE *fp = fopen(fname, "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    fprintf(fp, "c random 3-SAT\n");
    fprintf(fp, "c n=%d alpha=%f\n", formula.n, formula.alpha);
    fprintf(fp, "p cnf %d %d\n", formula.n, formula.m);
    for (int i = 0; i < formula.m; i++)
    {
        for (int j = 0; j < K; j++)
        {
            fprintf(fp, "%d ", formula.list[i].literal[j] * (formula.list[i].is_inverted[j] ? -1 : 1));
        }
        fprintf(fp, "0\n");
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s n alpha seed\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    double alpha = atof(argv[2]);
    unsigned int seed = atoi(argv[3]);
    srand(seed);

    if (n > MAXN)
    {
        fprintf(stderr, "n too large\n");
        exit(1);
    }
    if (alpha > MAXALPHA)
    {
        fprintf(stderr, "too many clauses\n");
        exit(1);
    }

    cnf_t formula;
    cnf_gen(&formula, n, alpha);

    char filename[100];
    snprintf(filename, 100, "n-%d_alpha-%f_seed-%d.cnf", n, alpha, seed);

    cnf_to_dimacs(formula,filename);
    printf("Output: %s\n", filename);

    return 0;
}

