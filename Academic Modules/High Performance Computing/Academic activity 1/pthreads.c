#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 8

double a = 0.0, b = 100.0; // Intervalo de integração
int num_steps = 1000000;    // Número de passos de integração
double step;                // Tamanho do passo
double total = 0.0;         // Resultado total da integração
pthread_mutex_t mutex;      // Mutex para sincronização

// Função a ser integrada
double func(double x) {
    return sqrt(100 * 100 - x * x);
}

// Função que cada thread executará
void *work(void *thread_id) {
    long tid;
    double local_sum = 0.0;
    int i;

    tid = (long)thread_id;
    for (i = tid; i < num_steps; i += NUM_THREADS) {
        double x = a + (i + 0.5) * step;
        local_sum += func(x);
    }

    pthread_mutex_lock(&mutex);
    total += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    long t;

    step = (b - a) / num_steps;

    pthread_mutex_init(&mutex, NULL);

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, work, (void *)t);
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    total *= step;

    printf("Resultado: %.10lf\n", total);

    pthread_mutex_destroy(&mutex);

    return 0;
}
