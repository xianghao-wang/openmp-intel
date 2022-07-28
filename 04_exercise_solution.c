#include <stdio.h>
#include <omp.h>

#define MAX_THREADS_N 10
static long num_steps = 1000;
double step;

int main()
{
    int i;
    int n_threads;
    double sum[MAX_THREADS_N];
    double pi;
    step = 1.0 / (double) num_steps;

    // Set the number of threads.
    omp_set_num_threads(MAX_THREADS_N);

    double start_time = omp_get_wtime();
    #pragma omp parallel
    {  
        int id = omp_get_thread_num();
        int i;
        // Actual threads may be fewer, so we need get the number of running threads
        int n_actual_threads = omp_get_num_threads();
        double x;

        // Update threads number
        if (id == 0) n_threads = n_actual_threads;

        // Compute in cyclic
        for (i = id, sum[id] = 0.0; i < num_steps; i += n_actual_threads) {
            x = i * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }

    for (i = 0, pi = 0.0; i < n_threads; i ++) 
    {
        pi += sum[i] * step;
    }

    printf("PI is %8.5lf, computed by %3d threads, taking %8.5lf seconds.\n",
        pi, n_threads, omp_get_wtime() - start_time);

    return 0;
}