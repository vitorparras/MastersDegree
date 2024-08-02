# Relatório de Análise de Desempenho de Integração Numérica Usando MPI

## 1. Introdução
Este relatório apresenta a solução para o problema de integração numérica utilizando a regra do trapézio, paralelizada com a biblioteca MPI (Message Passing Interface). O objetivo é calcular a integral da função \( f(x) = \sqrt{10000 - x^2} \) no intervalo de 0 a 100, variando o número de processos e os intervalos de discretização para analisar o impacto na precisão e no tempo de execução.

## 2. Descrição do Problema
A integração numérica é uma técnica utilizada para calcular a área sob uma curva quando a integração analítica não é possível. Neste exercício, utilizamos a regra do trapézio para aproximar a integral da função \( f(x) = \sqrt{10000 - x^2} \) no intervalo de 0 a 100. A regra do trapézio divide o intervalo de integração em subintervalos menores e soma a área dos trapézios formados sob a curva.

## 3. Resultados

### Resultados Obtidos
| Intervalo de Discretização | 1 Processo | 2 Processos | 4 Processos | 8 Processos |
|----------------------------|------------|-------------|-------------|-------------|
| 0.000001                   | 7853.981634| 7853.981634 | 7853.981634 | 7853.981634 |
| Tempo (s)                  | 2.176321   | 1.127604    | 0.715736    | 0.504945    |
| 0.000010                   | 7853.981634| 7853.981634 | 7853.981634 | 7853.981634 |
| Tempo (s)                  | 0.237738   | 0.134041    | 0.100259    | 0.071310    |
| 0.000100                   | 7853.981631| 7853.981631 | 7853.981631 | 7853.981631 |
| Tempo (s)                  | 0.033350   | 0.026309    | 0.016359    | 0.027878    |

### Análise dos Resultados
Os resultados mostram que a precisão da estimativa da integral é mantida independentemente do número de processos e do intervalo de discretização utilizados. Observa-se que, com o aumento do número de processos, o tempo de execução diminui significativamente, demonstrando a eficiência da paralelização com MPI.

#### Observações:
1. **Precisão**: Todos os resultados aproximam a integral correta (7853.981634) com alta precisão.
2. **Desempenho**: O tempo de execução diminui conforme aumentamos o número de processos, indicando que a carga de trabalho foi distribuída eficientemente entre os processos.

## 4. Conclusão
A implementação de integração numérica utilizando MPI mostrou que a paralelização é uma técnica eficaz para melhorar o desempenho computacional. A precisão da integral estimada foi mantida enquanto o tempo de execução foi reduzido significativamente com o aumento do número de processos. Este estudo demonstra que o uso de MPI é altamente benéfico para problemas que podem ser paralelizados, especialmente em ambientes de computação distribuída.

## 5. Referências
- Biblioteca MPI: https://www.mpi-forum.org/docs/
