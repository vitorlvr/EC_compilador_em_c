#ifndef FOO_H_
#define FOO_H_
#define MAX_LENGTH 100
#define buffer_size 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Interface das funções
typedef struct fila_no FilaNo;
typedef struct fila Fila;

Fila* fila_cria(void);
FILE* abrir_arquivo();
void fila_insere(Fila* f, char v[]);
char fila_remove(Fila* f);
void fila_libera(Fila *f);
void fila_exibe(Fila *f);
void remove_ultimo(Fila *comandos);
void ler_arquivo(FILE *arquivo, Fila *comandos);
void analise_funcao(Fila *comandos, FILE *analise);
int verifica_pos(char linha[], int quantidade);
int verifica_value(char linha[], int quantidade);
int verifica_offset(char linha[]);
void read(char linha[], int Nlinha, FILE *analise);
void write(char linha[], int Nlinha, FILE *analise);
void storeconst(char linha[], int Nlinha, FILE *analise);
void add(char linha[], int Nlinha, FILE *analise);
void sub(char linha[], int Nlinha, FILE *analise);
void multp(char linha[], int Nlinha, FILE *analise);
void divisao(char linha[], int Nlinha, FILE *analise);
void jump(char linha[], int Nlinha, FILE *analise);
void fstore(char linha[], int Nlinha, FILE *analise);
int conta_parametro(char linha[], int quantidade);

#endif