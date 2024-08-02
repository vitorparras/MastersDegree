#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    if (x > 100 || x < -100) {
        return 0.0;  // fora do domínio da função
    }
    return sqrt(10000 - x * x);
}

void run_experiment(double interval_size, int num_procs, int rank) {
    double a = 0.0, b = 100.0;
    double h = interval_size;
    long long n = (b - a) / h;
    long long local_n = n / num_procs;
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_integral = 0.0;

    // Verificação adicional para garantir que não haja erros
    if (n == 0 || local_n == 0) {
        if (rank == 0) {
            printf("Erro: Número de subintervalos locais é zero. Intervalo de discretização é muito grande ou número de processos é muito pequeno.\n");
        }
        return;
    }

    // Correção do cálculo dos trapézios
    for (long long i = 0; i < local_n; i++) {
        double x1 = local_a + i * h;
        double x2 = x1 + h;
        double f1 = f(x1);
        double f2 = f(x2);
        if (isnan(f1) || isnan(f2)) {
            printf("Rank %d: NaN detected in function evaluation at x1=%.6f, x2=%.6f\n", rank, x1, x2);
        }
        local_integral += (f1 + f2) * h / 2.0;
    }

    double integral = 0.0;
    MPI_Reduce(&local_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Com intervalos de discretização = %.6f e %d processos, nossa estimativa\n", interval_size, num_procs);
        printf("da integral de %.0f para %.0f = %.9f\n", a, b, integral);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    double intervals[] = {0.000001, 0.00001, 0.0001};
    int num_processes[] = {1, 2, 4, 8};

    for (int p = 0; p < 4; p++) {
        if (num_procs >= num_processes[p]) {
            for (int i = 0; i < 3; i++) {
                double interval_size = intervals[i];
                if (rank == 0) {
                    printf("Rodando com %d processos...\n", num_processes[p]);
                }
                double start_time = MPI_Wtime();
                run_experiment(interval_size, num_processes[p], rank);
                double end_time = MPI_Wtime();
                if (rank == 0) {
                    printf("Tempo gasto: %f segundos\n\n", end_time - start_time);
                }
                MPI_Barrier(MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
