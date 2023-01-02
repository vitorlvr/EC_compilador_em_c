#ifndef FOO_H_
#define FOO_H_
#define MAX_LENGTH 50
#define buffer_size 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Interface das funções
typedef struct fila_no FilaNo;
typedef struct fila Fila;

Fila* fila_cria(void);
FILE* abrir_arquivo(char *argv[]);
void fila_insere(Fila* f, char v[]);
char fila_remove(Fila* f);
void fila_libera(Fila *f);
void fila_exibe(Fila *f);
void ler_arquivo(FILE *arquivo, Fila *comandos);
void formata_linha(char linha[], Fila *comandos);
void analise_funcao(Fila *comandos, FILE *analise);
int verifica_pos(char linha[], int quantidade);
int verifica_value(char linha[], int quantidade);
int verifica_offset(char linha[], int Nlinha, int maxlinhas);
void verifica_op_aritmetica(char linha[], int Nlinha, FILE *analise, char comando[]);
void verifica_write_read_store(char linha[], int Nlinha, FILE *analise, char comando[]);
void read(char linha[], int Nlinha, FILE *analise);
void write(char linha[], int Nlinha, FILE *analise);
void storeconst(char linha[], int Nlinha, FILE *analise);
void add(char linha[], int Nlinha, FILE *analise);
void sub(char linha[], int Nlinha, FILE *analise);
void multp(char linha[], int Nlinha, FILE *analise);
void divisao(char linha[], int Nlinha, FILE *analise);
void jump(char linha[], int Nlinha, FILE *analise, Fila *comandos);
void fstore(char linha[], int Nlinha, FILE *analise);
int conta_parametro(char linha[], int quantidade);
void enumera_linhas(Fila *comandos);

#endif