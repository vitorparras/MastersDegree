#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

double a = 0.0, b = 100.0; // Intervalo da integral
double h; // Largura de cada intervalo
double integral = 0.0;
pthread_mutex_t mutex;

double f(double x) {
    return sqrt(10000 - x * x);
}

typedef struct {
    double local_a;
    double local_b;
    long long local_n;
} ThreadData;

void* thread_func(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    double local_a = data->local_a;
    double local_b = data->local_b;
    long long local_n = data->local_n;
    double local_integral = (f(local_a) + f(local_b)) / 2.0;
    double x;

    for (long long i = 1; i < local_n; i++) {
        x = local_a + i * h;
        local_integral += f(x);
    }
    local_integral *= h;

    pthread_mutex_lock(&mutex);
    integral += local_integral;
    pthread_mutex_unlock(&mutex);

    free(data);
    return NULL;
}

void run_experiment(double interval_size, int num_threads) {
    pthread_t threads[num_threads];
    pthread_mutex_init(&mutex, NULL);

    long long n = (b - a) / interval_size;
    h = interval_size;

    integral = 0.0; // Reset integral for each run
    long long intervals_per_thread = n / num_threads;
    double local_a = a;

    for (int thread = 0; thread < num_threads; thread++) {
        ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
        data->local_a = local_a;
        data->local_n = intervals_per_thread;
        data->local_b = local_a + intervals_per_thread * h;

        if (thread == num_threads - 1) { // Última thread pega qualquer intervalo restante
            data->local_b = b;
            data->local_n = (b - local_a) / h;
        }

        pthread_create(&threads[thread], NULL, thread_func, data);
        local_a = data->local_b;
    }

    for (int thread = 0; thread < num_threads; thread++) {
        pthread_join(threads[thread], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Com intervalos de discretizacao = %f e %d threads, nossa estimativa\n", interval_size, num_threads);
    printf("da integral de %d para %f = %f\n", a, b, integral);
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
            printf("Tempo gasto: %f segundos\n\n\n\n", time_spent);
        }
    }

    return 0;
}
