#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main() {

    Fila *comandos = fila_cria(); // Fila que vai armazenar em cada nó, uma linha do arquivo txt
    FILE *arquivo = abrir_arquivo(); // Função que abre o documento txt. Obs.: Por algum motivo, funciona para alguns e não para outros
    FILE *analise = fopen("C:\\Users\\2211014\\Desktop\\Estudo_de_caso_1\\analise.log", "w"); // Criando o arquivo log de análise
    if (analise == NULL)               // Alterar o diretório do '.log' caso de erro ^
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