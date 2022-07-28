#include <stdio.h>
#include <omp.h>

/*
    f(x)=4/(1+x^2)
    Calculate integral of f from 0 to 1
*/

double f(double x)
{
    return 4.0 / (1 + x * x);
}

int min(int a, int b)
{
    return a > b ? b : a;
}

double subAreas[1024];

int main()
{
    long steps = 1000000000;
    double width = 1.0 / steps;
    int threads = 10;
    omp_set_num_threads(threads);

    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        int rank = omp_get_thread_num();
        int leftBoundary = rank * steps / threads;
        int rightBoundary = min(steps, (rank + 1) * steps / threads);
        
        double area = 0.0;
        for (int i = leftBoundary; i < rightBoundary; i ++)
        {
            area += f(i * width) * width;
        }
        subAreas[rank] = area;
    }

    // Sum sub-results
    double area = 0.0;
    for (int i = 0; i < threads; i ++)
    {
        area += subAreas[i];
    }

    double duration = omp_get_wtime() - startTime;

    printf("Area is %8.5lf, computed by %3d threads, taking %8.5lf seconds.\n",
        area, threads, duration);


    return 0;
}