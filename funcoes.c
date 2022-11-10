#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

struct fila_no {
    char info[25]; // Vetor que armazena cada linha do txt
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

void remove_ultimo(Fila *comandos)
{
    FilaNo *ptr = comandos->primeiro;
    Fila *ptrF = comandos->ultimo;
    int contador = 0;

    while (1)
    {
        if(ptr->proximo == ptrF)
            break;
        contador++;
        ptr = ptr->proximo;
    }
    free(ptrF);
    comandos->ultimo = ptr;
    comandos->ultimo->proximo = NULL;
}

FILE *abrir_arquivo()
{
    char diretorio[buffer_size];
    int contador, j = 0;
    printf("Digite o endereco do arquivo: ");
    gets(diretorio);
    contador = strlen(diretorio);
    char diretorio2[contador];
    for (int i = 0; i<contador; i++)
        diretorio2[i] = diretorio[i];
    FILE *arquivo = fopen(diretorio2, "r");
    
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
    int i = 0, contador = 0, j = 0;
    char buffer[buffer_size];
    for (i = 0; !feof(arquivo); i++)
    {
        j++;
        fgets(buffer, buffer_size, arquivo);
        if (buffer[0]=='\n')
            continue;
        contador = strlen(buffer);
        char buffer2[contador - 1];
        for (i = 0; i<contador; i++)
            buffer2[i] = buffer[i];
        fila_insere(comandos, buffer2);
    }
    if (j>1)
        remove_ultimo(comandos); // A ultima linha fica duplicada no final da fila. Por isso a necessidade de remover o ultimo nó
}

void analise_funcao(Fila *comandos, FILE *analise)
{
    FilaNo *ptr = comandos->primeiro;
    int linha = 1;

    while (ptr != NULL)
    {
        int contador = 0, teste = 0;
        for (int i = 0; i < 25; i++)
        {
            if (ptr->info[i] != ' ' && ptr->info[i] != '\n')
            {
                contador++;
                continue;
            }
            break;
        }
        char funcao[contador];
        for (int i = 0; i < contador; i++)
        {
            funcao[i] = ptr->info[i];
        }
        if(strncmp(funcao, "read", contador) == 0) // Caso a comparação seja igual a '0', a função da linha é 'read' por exemplo
        {
            read(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++; // Armazenando a linha em que se encontra a função a fim de que, caso ocorra algum erro, conseguir apontar a linha
            continue; // Próxima função
        }
        if(strncmp(funcao, "write", contador) == 0)
        {
            write(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "storeconst", contador) == 0 && contador != 5)
        {
            storeconst(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "add", contador) == 0)
        {
            add(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "sub", contador) == 0)
        {
            sub(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "mul", contador) == 0)
        {
            multp(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "div", contador) == 0)
        {
            divisao(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "store", contador) == 0)
        {
            fstore(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(strncmp(funcao, "jump", contador) == 0)
        {
            jump(ptr->info, linha, analise);
            ptr->linha = linha;
            ptr = ptr->proximo;
            teste = 1;
            linha++;
            continue;
        }
        if(teste == 0) // Se a variável teste continuar com seu valor inicial, a função não pertence a nenhuma das préestabelecidas
        {
            fprintf(analise, "Comando da linha %d nao reconhecido!\n", linha);
            break;
        }
        ptr = ptr->proximo;
    }
}

int verifica_value(char linha[], int quantidade) // Verificando se o número é valido
{
    int inicio = 0, fim = 0, j = 0, i, tam = 0, contador = 0, resposta = 0;
    for (i = 0; i < strlen(linha); i++) // Analisando se o primeiro numero é um numero válido
    {
        if ((linha[i] == ' ' || linha[i] == '\n') && inicio != 0)
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
        if ((linha[i] == ' ' || linha[i] == '\n') && inicio != 0)
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
        if ((linha[i] == ' ' || linha[i] == '\n') && inicio != 0)
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

int verifica_offset(char linha[]) // Verificando se é inteiro
{
    int inicio = 0, fim = 0, j = 0, i, tam = 0, contador = 0, resposta = 0;
    for (i = 0; i < strlen(linha); i++) // Pulando o primeiro numero;
    {
        if ((linha[i] == ' ' || linha[i] == '\n') && inicio != 0)
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
    for (i = fim-1; i < strlen(linha); i++) // Analisando o segundo numero(offset), que deve ser inteiro;
    {
        if ((linha[i] == ' ' || linha[i] == '\n') && inicio != 0)
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
        if ((numero2[i] >= 48 && numero2[i] <= 57) && numero2[i] != '.' && numero2[i+1] != '-')
        {
            continue;
        }
        if (numero2[0] == '-' && i == 0)
            continue;
        resposta = 2; // Erro no segundo parâmetro
    }
    return resposta;
}

int conta_parametro(char linha[], int quantidade)
{
    char vetor[strlen(linha)];
    int contador = 0;
    for (int i = 0; i < strlen(linha); i++)
    {
        vetor[i] = linha[i];
        if (vetor[i] == 10)
            break;
        if (vetor[i] == ' ' && vetor[i+1] != ' ')
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
        fprintf(analise, "Comando read executado\n"); // Comando foi executado
}

void write(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando write executado\n");
    else
        printf("Erro no comando 'write'\n");
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
        fprintf(analise, "Comando storeconst executado\n");
    else
        printf("Erro no comando 'storeconst'\n");
}

void add(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando add executado\n");
    else
        printf("Erro no comando 'add'\n");
}               

void sub(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando sub executado\n");
    else
        printf("Erro no comando 'sub'\n");
}

void multp(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando mul executado\n");
    else
        printf("Erro no comando 'mul'\n");
}

void divisao(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando div executado\n");
    else
        printf("Erro no comando 'div'\n");
}

void jump(char linha[], int Nlinha, FILE *analise)
{
    int resposta1, resposta2, parametros;
    resposta1 = verifica_pos(linha, 1);
    if (resposta1 == 1)
        fprintf(analise, "Linha %d: erro no parametro 1 'pos', que deve ser um numero natural.\n", Nlinha);
    resposta2 = verifica_offset(linha);
    if (resposta2 == 2)
        fprintf(analise, "Linha %d: erro no parametro 2 'pos', que deve ser um numero natural.\n", Nlinha);
    parametros = conta_parametro(linha, 2);
    if (parametros == 1)
        fprintf(analise, "Linha %d: numero de parametros excedido\n", Nlinha);
    if (parametros == -1)
        fprintf(analise, "Linha %d: falta de parametros necessarios\n", Nlinha);
    if((resposta1 == 0 && resposta2 == 0) && parametros == 0)
        fprintf(analise, "Comando jump executado\n");
    else
        printf("Erro no comando 'jump'\n");
}

void fstore(char linha[], int Nlinha, FILE *analise)
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
        fprintf(analise, "Comando store executado\n");
    else
        printf("Erro no comando 'store'\n");
}