#ifndef LEXICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>

#define FILA_MAX 256

void analisador_lexico(FILE *codigo);
int DefineLexema(char *fila, int tipo);
void CriaTabela(FILE *saida, int pos, char *fila, int tipoLexema, int col, int row);
int isSeparador(char c);
int isReservada(char *fila);
int isOperador(char c);
int isOperadorDuplo(const char *token);
#endif