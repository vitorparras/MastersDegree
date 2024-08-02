#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

double a = 0.0, b = 100.0; // Intervalo da integral

double f(double x) {
    return sqrt(10000 - x * x);
}

void run_experiment(double interval_size, int num_threads) {
    double h = interval_size;
    long long n = (b - a) / h;
    double integral = 0.0;

    #pragma omp parallel for num_threads(num_threads) reduction(+:integral)
    for (long long i = 0; i < n; i++) {
        double x = a + i * h;
        integral += f(x) * h;
    }

    // Correção para somar os termos finais da regra do trapézio
    integral += (f(a) + f(b)) * h / 2.0;
    
    printf("Com intervalos de discretizacao = %f e %d threads, nossa estimativa\n", interval_size, num_threads);
    printf("da integral de %f para %f = %f\n", a, b, integral);
}

int main() {
    double intervals[] = {0.000001, 0.00001, 0.0001};
    int threads[] = {1, 2, 4, 8};

    for (int j = 0; j < 4; j++) {  // Loop para o número de threads
        for (int i = 0; i < 3; i++) {  // Loop para os intervalos de discretização
            double interval_size = intervals[i];
            printf("Rodando com intervalos de discretizacao = %f e %d threads...\n", interval_size, threads[j]);
            clock_t start = clock();
            run_experiment(interval_size, threads[j]);
            clock_t end = clock();
            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Tempo gasto: %f segundos\n\n", time_spent);
        }
    }

    return 0;
}
