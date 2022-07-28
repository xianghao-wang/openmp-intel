#include <stdio.h>
#include <omp.h>

#define MAX_THREADS_N 5
#define PADDING 8
static long num_steps = 1000000000;
double step;

int main()
{
    int i;
    int n_threads;

    /*
        Each threads may share the same cache line, 
        causing False Sharing.
        This can be sovled by giving the shared array padding
        to avoid them sharing the same cache line.
    */
    double sum[MAX_THREADS_N][PADDING];
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
        for (i = id, sum[id][0] = 0.0; i < num_steps; i += n_actual_threads) {
            x = i * step;
            sum[id][0] += 4.0 / (1.0 + x * x);
        }
    }

    for (i = 0, pi = 0.0; i < n_threads; i ++) 
    {
        pi += sum[i][0] * step;
    }

    printf("PI is %8.5lf, computed by %3d threads, taking %8.5lf seconds.\n",
        pi, n_threads, omp_get_wtime() - start_time);

    return 0;
}