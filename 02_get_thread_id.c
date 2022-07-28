#include <omp.h>
#include <stdio.h>

int main()
{
    #pragma omp parallel
    {
        // Get ID of the running thread
        int ID = omp_get_thread_num();
        
        printf("hello(%d)", ID);
        printf(" world(%d)\n", ID);
    }

    printf("done\n");

    return 0;
}