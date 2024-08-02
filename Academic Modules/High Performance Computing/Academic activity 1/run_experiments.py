import os
import subprocess

# Configurações de threads e intervalos de discretização
thread_counts = [1, 2, 4, 8]
discretization_intervals = [0.000001, 0.00001, 0.0001]
programs = {
    "pthreads": "./output/integral_pthreads",  # Atualize o caminho se necessário
    "openmp": "./output/integral_openmp"       # Atualize o caminho se necessário
}
n_values = [int((100.0 / h)) for h in discretization_intervals]

# Função para executar o programa e coletar o tempo de execução
def run_program(program, n, num_threads=None):
    env = os.environ.copy()
    if num_threads:
        env["OMP_NUM_THREADS"] = str(num_threads)
    result = subprocess.run(
        [program, str(n)],
        capture_output=True,
        text=True,
        env=env
    )
    return result.stdout, result.stderr

# Verificar se os executáveis estão presentes
for program_name, program_path in programs.items():
    if not os.path.isfile(program_path):
        raise FileNotFoundError(f"Executable for {program_name} not found at path: {program_path}")

# Diretório para armazenar os resultados
output_dir = "results"
os.makedirs(output_dir, exist_ok=True)

# Executar os programas
for program_name, program_path in programs.items():
    for num_threads in thread_counts:
        for h, n in zip(discretization_intervals, n_values):
            output_file = os.path.join(output_dir, f"{program_name}_threads_{num_threads}_interval_{h}.txt")
            print(f"Executando {program_name} com {num_threads} threads e intervalo {h}")
            stdout, stderr = run_program(program_path, n, num_threads)
            with open(output_file, "w") as f:
                f.write(stdout)
                if stderr:
                    f.write("\nSTDERR:\n")
                    f.write(stderr)
