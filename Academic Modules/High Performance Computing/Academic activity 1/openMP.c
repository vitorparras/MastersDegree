#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define NUM_THREADS 8

double a = 0.0, b = 100.0; // Intervalo de integração
int num_steps = 1000000;    // Número de passos de integração
double step;                // Tamanho do passo
double total = 0.0;         // Resultado total da integração

// Função a ser integrada
double func(double x) {
    return sqrt(100 * 100 - x * x);
}

int main() {
    int i;
    double x;

    step = (b - a) / num_steps;
    total = 0.0;

    #pragma omp parallel num_threads(NUM_THREADS) private(i, x) shared(total)
    {
        double local_sum = 0.0;

        #pragma omp for
        for (i = 0; i < num_steps; i++) {
            x = a + (i + 0.5) * step;
            local_sum += func(x);
        }

        #pragma omp critical
        total += local_sum;
    }

    total *= step;

    printf("Resultado: %.10lf\n", total);

    return 0;
}
