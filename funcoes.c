#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

struct fila_no {
    char info[30]; // Vetor que armazena cada linha do txt
    int linha; // Linha do documento salva no nó
    FilaNo* proximo;
};


struct fila {
    FilaNo* primeiro;
    FilaNo* ultimo;
};

Fila* fila_cria(void)
{
    Fila *f = (Fila *) malloc(sizeof(Fila));
    f->primeiro = NULL;
    f->ultimo = NULL;
    return f;
}

void fila_insere(Fila* f, char v[])
{
    FilaNo *novo = (FilaNo*) malloc(sizeof(FilaNo));
    strcpy(novo->info, v);
    novo->proximo = NULL;
    if (f->ultimo != NULL)
        f->ultimo->proximo = novo;
    else
        f->primeiro = novo;
    f->ultimo = novo;
}

char fila_remove(Fila* f)
{
    FilaNo *primeiro = f->primeiro;
    if (primeiro == NULL)
        return 0;
    
    FilaNo *segundo = primeiro->proximo;
    f->primeiro = segundo;
    if (f->primeiro == NULL)
        f->ultimo = NULL;
    char valor = primeiro->info;
    free(primeiro);
    return valor;
}

void fila_libera(Fila *f)
{
    FilaNo *f_aux = f->primeiro;
    while (f_aux != NULL)
    {
        FilaNo *t  = f_aux->proximo;
        free(f_aux);
        f_aux = t;
    }
    free(f);
}

void fila_exibe(Fila *f)
{
    FilaNo *ptr = f->primeiro;
    printf("\nFila: ");
    while(ptr != NULL)
    {
        printf("%d ", ptr->info);
        ptr = ptr->proximo;
    }
    printf("\n");
}

FILE *abrir_arquivo(char *argv[])
{
    FILE *arquivo = fopen(argv[1], "r");
    
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }
    printf("Arquivo aberto\n");
    return arquivo;
}

void ler_arquivo(FILE *arquivo, Fila *comandos)
{ 
    char buffer[buffer_size];
    int i = 0, j;
    while (!feof(arquivo))
    {
        fgets(buffer, buffer_size, arquivo);
        char buffer2[strlen(buffer)-1];
        strcpy(buffer2, buffer);
        formata_linha(buffer2, comandos);
    }
}

void formata_linha(char linha[], Fila *comandos)
{
    int i = 0, j = 0, inicio, fim;
    char funcao[30];
    for(i = 0; i < strlen(linha); i++)
    {
        while (linha[i] == ' ')
            i++;
        if (linha[i] == '#')
            break;
        else if (linha[i] == '\n')
        {
            funcao[j] = '\0';
            break;
        }
        while (linha[i] > 32 && linha[i] < 127) // Removendo lixo
        {
            funcao[j] = linha[i];
            i++;
            j++;
        }
        if (linha[i] != '\0')
        {
            funcao[j] = 32;
            j++;
        }
        if (j > buffer_size)
            break;
        continue;
    }
    funcao[j-1] = '\0';
    char funcao2[strlen(funcao)];
    strcpy(funcao2, funcao);
    for (int j = 0; j < strlen(funcao2); j++)
        if ((funcao2[j] > 32 && funcao2[j] <= 126) && strlen(funcao2) > 3)
        {
            fila_insere(comandos, funcao2);
            break;
        }
}

void enumera_linhas(Fila *comandos)
{
    FilaNo *ptr = comandos->primeiro;
    int linha = 1;
    while (ptr != NULL)
    {
        ptr->linha = linha;
        linha++;
        ptr = ptr->proximo;
    } 
}

void analise_funcao(Fila *comandos, FILE *analise)
{
    FilaNo *ptr = comandos->primeiro;
    int linha = 1, j = 0;

    enumera_linhas(comandos);
    while (ptr != NULL)
    {
        int contador = 0, i;
        int Nlinha = ptr->linha;
        for (i = 0; i < strlen(ptr->info); i++)
        {
            if (ptr->info[i] > 96 && ptr->info[i] < 123)
                contador++;
            else
                break;
        }
        char funcao[contador];
        for (int i = 0; i < contador; i++)
        {
            funcao[i] = ptr->info[i];
        }
        if(strncmp(funcao, "read", contador) == 0) // Caso a comparação seja igual a '0', a função da linha é 'read' por exemplo
        {
            verifica_write_read_store(ptr->info, Nlinha, analise, "read");
        } 
        else if(strncmp(funcao, "write", contador) == 0)
        {
            verifica_write_read_store(ptr->info, Nlinha, analise, "write");
        }
        else if(strncmp(funcao, "store", contador) == 0)
        {
            verifica_write_read_store(ptr->info, Nlinha, analise, "store");
        }
        else if(strncmp(funcao, "storeconst", contador) == 0 && contador != 5)
        {
            storeconst(ptr->info, Nlinha, analise);
        }
        else if(strncmp(funcao, "add", contador) == 0)
        {
            verifica_op_aritmetica(ptr->info, Nlinha, analise, "add");
        }
        else if(strncmp(funcao, "sub", contador) == 0)
        {
            verifica_op_aritmetica(ptr->info, Nlinha, analise, "sub");
        }
        else if(strncmp(funcao, "mul", contador) == 0)
        {
            verifica_op_aritmetica(ptr->info, Nlinha, analise, "mult");
        }
        else if(strncmp(funcao, "div", contador) == 0)
        {
            verifica_op_aritmetica(ptr->info, Nlinha, analise, "div");
        }
        else if(strncmp(funcao, "jump", contador) == 0)
        {
            jump(ptr->info, Nlinha, analise, comandos);
        }
        else
        {
            fprintf(analise, "Comando da linha %d nao reconhecido!\n", Nlinha);
        } // Armazenando a linha em que se encontra a função a fim de que, caso ocorra algum erro, conseguir apontar a linha
        ptr = ptr->proximo;
    }
}

void verifica_write_read_store(char linha[], int Nlinha, FILE *analise, char comando[])
{
    int resposta, parametros;
    resposta = verifica_pos(linha, 1);
    if (resposta == 1)
        fprintf(analise, "Linha %d: erro no parametro 1 'pos', que deve ser um numero natural.\n", Nlinha);
        parametros = conta_parametro(linha, 1);
    if (parametros == 1)
        fprintf(analise, "Linha %d: numero de parametros excedido\n", Nlinha);
    if (parametros == -1)
        fprintf(analise, "Linha %d: falta de parametros necessarios\n", Nlinha);
    if(resposta == 0 && parametros == 0)
        fprintf(analise, "Comando '%s' executado\n", comando);
    else
        printf("Erro no comando '%s'\n", comando);
}

void verifica_op_aritmetica(char linha[], int Nlinha, FILE *analise, char comando[])
{
    int resposta, parametros;
    resposta = verifica_pos(linha, 2);
    if (resposta == 1)
        fprintf(analise, "Linha %d: erro no parametro 1 'pos', que deve ser um numero natural.\n", Nlinha);
    if (resposta == 2)
        fprintf(analise, "Linha %d: erro no parametro 2 'pos', que deve ser um numero natural.\n", Nlinha);
        parametros = conta_parametro(linha, 2);
    if (parametros == 1)
        fprintf(analise, "Linha %d: numero de parametros excedido\n", Nlinha);
    if (parametros == -1)
        fprintf(analise, "Linha %d: falta de parametros necessarios\n", Nlinha);
    if(resposta == 0 && parametros == 0)
        fprintf(analise, "Comando '%s' executado\n", comando);
    else
        printf("Erro no comando '%s'\n", comando);
}

int verifica_value(char linha[], int quantidade) // Verificando se o número é valido
{
    int inicio = 0, fim = 0, j = 0, i, tam = 0, contador = 0, resposta = 0;
    for (i = 0; i < strlen(linha); i++) // Analisando se o primeiro numero é um numero válido
    {
        if (linha[i] == ' ' && inicio == 0)
        {
            inicio = i+1;
            continue;
        } else if (linha[i] == '-' && inicio == 0)
            continue;
        if (linha[i] == ' ' && inicio != 0)
        {
            fim = i;
            break;
        }
    }
    tam = fim - inicio;
    char numero1[tam]; // Por conta de não ter nenhuma função que utiliza 'value' como segundo parâmetro, não há a necessidade de se fazer a análise
    for (i = inicio; i < fim; i++)
    {
        numero1[j] = linha[i];
        j++;
    }
    for (i = 0; i < tam; i++)
    {
        if ((numero1[i] >= 48 && numero1[i] <= 57) && numero1[0] != '.' && numero1[i+1] != '-')
        {
            continue;
        }
        if (numero1[0] == '-' && numero1[1] == '.')
        {
            resposta = 1; // Número inválido
        }
        if (numero1[0] == '-')
            continue;
        resposta = 0; // Número válido
    }

    return resposta;
}

int verifica_pos(char linha[], int quantidade) // Verificando se é um número natural
{
    int inicio = 0, fim = 0, j = 0, i, tam = 0, contador = 0, resposta = 0; // Variável 'resposta' iniciando com valor 0, que significa que o parâmetro é valido

    for (i = 0; i < strlen(linha); i++) // Analisando se o primeiro numero é um numero válido
    {
        if ((linha[i] == '\0') && inicio != 0)
        {
            fim = i;
            break;
        }
        if (linha[i] == ' ')
        {
            inicio = i+1;
            continue;
        }
    }
    tam = fim - inicio;
    char numero1[tam];
    for (i = inicio; i < fim; i++)
    {
        numero1[j] = linha[i];
        j++;
    }
    for (i = 0; i < tam; i++)
    {
        if (numero1[i] >= 48 && numero1[i] <= 57)
        {
            continue;
        } else {
            resposta = 1; // Erro no primeiro parâmetro
        }
    }
    if (quantidade <= 1) // Se a linha apresentar somente 1 parâmetro(pos), a função já se encerra por aqui
        return resposta;
    inicio = 0;
    j = 0;
    for (i = fim-1; i < strlen(linha); i++) // No caso de o comando apresentar dois numeros válidos
    {
        if ((linha[i] == '\0') && inicio != 0)
        {
            fim = i;
            break;
        }
        if (linha[i] == ' ')
        {
            inicio = i+1;
            continue;
        }
    }
    tam = fim - inicio;
    char numero2[tam];
    for (int i = inicio; i < strlen(linha); i++)
    {
        numero2[j] = linha[i];
        j++;
    }
    for (int i = 0; i < tam; i++)
    {
        if (numero2[i] >= 48 && numero2[i] <= 57)
        {
            continue;
        } else {
            resposta = 2; // Erro no segundo parâmetro
        }
    }
    return resposta;
}

int verifica_offset(char linha[], int Nlinha, int maxlinhas) // Verificando se é inteiro
{
    int inicio = 0, fim = 0, j = 0, i, tam = 0, contador = 0, resposta = 0;
    for (i = 0; i < strlen(linha); i++) // Pulando o primeiro numero;
    {
        if (linha[i] == ' ' && inicio != 0)
        {
            inicio++;
            break;
        }
        if (linha[i] == ' ')
        {
            inicio = i + 2;
            continue;
        }
    }
    for (i = inicio; i < strlen(linha); i++) // Analisando o segundo numero(offset), que deve ser inteiro;
    {
        if (linha[i] != ' ' && linha[i+1] == '\0')
        {
            fim = i;
            break;
        }
    }
    tam = fim - inicio;
    char numero2[tam];
    for (int i = inicio; i < strlen(linha); i++)
    {
        numero2[j] = linha[i];
        j++;
    }
    for (int i = 0; i < tam; i++)
    {
        if ((numero2[i] >= 48 && numero2[i] <= 57) && numero2[i] != '.' && numero2[i+1] != '-')
        {
            continue;
        }
        if (numero2[0] == '-' && i == 0)
            continue;
        resposta = 2; // Erro no segundo parâmetro
    }
    char *endptr;
    int numero = strtol(numero2, &endptr, 10);
    if (numero < -Nlinha || numero > (maxlinhas-Nlinha))
        resposta = 3;
    return resposta;
}

int conta_parametro(char linha[], int quantidade)
{
    int contador = 0;
    for (int i = 0; i < strlen(linha); i++)
    {
        if (linha[i+1] == '\0')
            break;
        else if (linha[i] == ' ')
            contador++;
    }
    if (contador == quantidade)
        return 0; // Válido
    if (contador > quantidade)
        return 1; // Inválido
    if (contador < quantidade)
        return -1; // Inválido
}

void read(char linha[], int Nlinha, FILE *analise) // Função de cada uma das funções do compilador (read, write, jump, ...)
{
    
}

void write(char linha[], int Nlinha, FILE *analise)
{
    
}

void storeconst(char linha[], int Nlinha, FILE *analise)
{
    int resposta1, resposta2, i = 0, parametros;
    resposta1 = verifica_value(linha, 1);
    resposta2 = verifica_pos(linha, 2);
    if(resposta1 == 1 && resposta2 == 2)
    {
        fprintf(analise, "Linha %d: erro nos dois parametros 'value' e 'pos'\n", Nlinha);
        i = 1;
    }
    if (resposta1 == 1 && i == 0)
        fprintf(analise, "Linha %d: erro no parametro 1 'value', que deve ser um valor numerico.\n", Nlinha);
    if (resposta2 == 2 && i == 0)
        fprintf(analise, "Linha %d: erro no parametro 2 'pos', que deve ser um numero natural.\n", Nlinha);
        parametros = conta_parametro(linha, 2);
    if (parametros == 1)
        fprintf(analise, "Linha %d: numero de parametros excedido\n", Nlinha);
    if (parametros == -1)
        fprintf(analise, "Linha %d: falta de parametros necessarios\n", Nlinha);
    if((resposta1 == 0 && resposta2 <= 1) && parametros == 0)
        fprintf(analise, "Comando 'storeconst' executado\n");
    else
        printf("Erro no comando 'storeconst'\n");
}

void add(char linha[], int Nlinha, FILE *analise)
{
    
}               

void sub(char linha[], int Nlinha, FILE *analise)
{
    
}

void multp(char linha[], int Nlinha, FILE *analise)
{
    
}

void divisao(char linha[], int Nlinha, FILE *analise)
{
    
}

void jump(char linha[], int Nlinha, FILE *analise, Fila *comandos) // Fazer o comando de pular ou voltar em funções
{
    int resposta1, resposta2, parametros;
    FilaNo *ptr = comandos->primeiro;
    while (ptr->proximo != NULL)
        ptr = ptr->proximo;
    resposta1 = verifica_pos(linha, 1);
    if (resposta1 == 1)
        fprintf(analise, "Linha %d: erro no parametro 1 'pos', que deve ser um numero natural.\n", Nlinha);
    resposta2 = verifica_offset(linha, Nlinha, ptr->linha);
    if (resposta2 == 2)
        fprintf(analise, "Linha %d: erro no parametro 2 'offset', que deve ser um numero natural.\n", Nlinha);
    if (resposta2 == 3)
        fprintf(analise, "Linha %d: offset inválido!\n", Nlinha);
    parametros = conta_parametro(linha, 2);
    if (parametros == 1)
        fprintf(analise, "Linha %d: numero de parametros excedido\n", Nlinha);
    if (parametros == -1)
        fprintf(analise, "Linha %d: falta de parametros necessarios\n", Nlinha);
    if((resposta1 == 0 && resposta2 == 0) && parametros == 0)
        fprintf(analise, "Comando 'jump' executado\n");
    else
        printf("Erro no comando 'jump'\n");
}

void fstore(char linha[], int Nlinha, FILE *analise)
{
    
}