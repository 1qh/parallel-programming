#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    int i, max;
    int a[10];

    for (i = 0; i < 10; i++){
        a[i] = rand() % 100;
        printf("%d\n", a[i]);
    }     
    
    max = a[0];

    #pragma omp parallel for 
    for (i = 1; i < 10; i++){
        if (a[i] > max) {

            #pragma omp critical
            {
                if (a[i] > max) max = a[i];
            }
        }
    }
    printf("max = %d\n", max);
}