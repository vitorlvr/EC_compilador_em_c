#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main(int argc, char *argv[]) // Faltou solucionar alguns erros. 1- Se der espaço antes da função, o compilador fica rodando infinitamente. De resto funciona corretamente. 2- Faltou arrumar o controle do offset
{
    Fila *comandos = fila_cria(); // Fila que vai armazenar em cada nó, uma linha do arquivo txt
    if (argc < 3)
    {
        printf("Faltam argumentos para a execucao do compilador");
        exit(EXIT_FAILURE);
    }
    FILE *arquivo = abrir_arquivo(argv); // Função que abre o documento txt.
    FILE *analise = fopen(argv[2], "w"); // Criando o arquivo log de análise
    
    if (analise == NULL)
    {
        printf("Erro ao abrir o arquivo '.log'");
        exit(EXIT_FAILURE);
    }
    ler_arquivo(arquivo, comandos); // Função que pega cada linha do documento, coloca em um vetor e armazena em cada nó da lista
    
    analise_funcao(comandos, analise); // Função que analisa cada nó. Ela pega somente a função presente na linha e manda para outra função, da qual se refere o comando

    printf("Analise finalizada! Verificar arquivo .log\n");
    fila_libera(comandos); // Liberando a lista da memória
    fclose(arquivo); // Fechando o arquivo e liberando a memória
    
    return 0;
}