#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 5

double plus2(int id, double *A)
{
    return A[id] + 2;
}

double mul5(int id, double *A)
{
    return A[id] * 5;
}

int main()
{
    
    double A[MAX_THREADS] = {0.0};

    omp_set_num_threads(MAX_THREADS);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        A[id] = plus2(id, A);
        
        #pragma omp barrier

        A[id] = mul5(id, A);
    }

    for (int i = 0; i < MAX_THREADS; i ++)
        printf("%g ", A[i]);
    putchar('\n');

    return 0;
}