#include "lexico.h"

const char *palavras_chave[] = {
    "if", "else", "do", "while", "for", "return",
    "int", "float", "char", "void", "struct",
    "break", "continue", "switch", "case", "printf","default", "main"
};
const char separadores[] = {' ', ',', ';', '\n', '\r', '(', ')'};
const char operadores[] = {'=', '+', '-', '/','*'};
const char *operadores_duplos[] = {"++", "--", "==", "||", };

void analisador_lexico(FILE *codigo){
    int pos = 0;
    int col = 0;
    int row = 0;

    char c = fgetc(codigo);
    FILE *saida = fopen("saida.txt", "w");
    while (c != EOF){
        int tipo;
        char fila[FILA_MAX] = {0};
        int cont = 0;
        
        while (isspace(c)) {
            if (c == '\n') {
                row++;
                col = 0;
            } if (c == '\t'){
                col +=4;
            } else {
                col++;
            }
            c = fgetc(codigo);
        }

        if (isalpha(c)){
            tipo = 1; //identificador
        } else if(isdigit(c)){
            tipo = 2; //numero
        } else if (isOperador(c)) {
            tipo = 3; //operador (se for - pode  ser número negativo)
        } else if(c == '\'' || c == '"'){
            tipo = 4; //identifica inicio de um literal
        } else if (c == '#'){
            tipo = 5; //identifica diretivas
        } else if (isSeparador(c)){
            tipo = 6; // separadores
        } else {
            tipo = 11;
        }
        
        
        
        fila[cont++] = c;

        while ((c = fgetc(codigo)) != EOF){
            if (tipo == 4) { 
                fila[cont++] = c;
                if (c == fila[0] && fila[0] != '\\') {
                    break;
                } 
                continue;
            }

            if (tipo == 5) { // Diretivas
                if (c == '\n' || c == '\r') {
                    break;
                }
                fila[cont++] = c;
                continue;
            }

            if (tipo == 1 && (isalnum(c) || c == '_')) {
                fila[cont++] = c;
            } else if (tipo == 2 && (isdigit(c) || c == '.')) {
                fila[cont++] = c;
            } else if (tipo == 3 && isOperador(c) && cont < 2) {
                fila[cont++] = c;
            } else {
                ungetc(c, codigo);
                break;
            }
            
        }

        int tipoLexema = DefineLexema(fila, tipo);
        if (tipoLexema != 10) { 
            CriaTabela(saida, pos++, fila, tipoLexema, col, row);
        }

        if (c == '\n') {
            row++;
            col = 0;
        } else if (c == '\t') {
            col += 4;
        } else {
            col++;
        }
        c = fgetc(codigo);
        
    }
    fclose(saida);
}

int DefineLexema(char *fila, int tipo){
    int tamanho = strlen (fila);
    //return 0: palavra-chave
    //return 1: identificador
    //return 2: numero
    //return 3: operador
    //return 7: literal
    //return 8: diretiva
    //return 9: separador
    //return 10: default
    //return 11: erro 
    if (tipo == 1){

        if(isReservada(fila)){
            return 0;
        }

        for (int i = 0; i < tamanho; i++){
            if (i != 0){
                if (!isalnum(fila[i]) && fila[i] != '_'){
                    return 11;
                }
            } 
        }

        return 1;
    }

    if (tipo == 2){
        for (int i = 0; i < tamanho; i++){
            if (!isdigit(fila[i])){
                return 11;
            }
        }
        return 2;
    }

    if (tipo == 3){
        if (isOperadorDuplo(fila)){
            return 3;
        }

        if (strlen (fila) == 1 && isOperador(fila[0])){
            return 3;
        }
        if (fila[0] == '-'){
            if (isdigit(fila[1])){
                return 2;
            }
        }
        return 11;
    }

    if (tipo == 4) { 
        if (tamanho >= 2 && fila[0] == fila[tamanho-1]) {
            return 7; 
        }
        return 11; 
    }

    if (tipo == 5) { 
        return 8; 
    }

    if (tipo == 6 && (fila[0] != '\n' || fila[0] != '\r' || fila[0] != '\t' || fila[0] != ' ')) { 
        return 9; 
    }

    return 10;
}

void CriaTabela(FILE *saida, int pos, char *fila, int tipoLexema, int col, int row){
    char *lexema;
    switch(tipoLexema) {
        case 0: lexema = "keyword"; break;
        case 1: lexema = "id"; break;
        case 2: lexema = "numero"; break;
        case 3: lexema = "operador"; break;
        case 7: lexema = "literal"; break;
        case 8: lexema = "diretiva"; break;
        case 9: lexema = "separador"; break;
        default: lexema = "erro"; break;
    }

    fprintf(saida, "%3d | %-40s | %-10s | Linha:%3d Col:%3d\n", 
        pos+1, fila, lexema, row, col);   
}


int isSeparador(char c) {
    int tamanho = sizeof(separadores)/sizeof(separadores[0]);
    for (size_t i = 0; i < tamanho; i++) {
        if (c == separadores[i]) {
            return 1;
        }
    }
    return 0;
}


int isReservada(char *fila){
    int tamanho = sizeof(palavras_chave) / sizeof(palavras_chave[0]);

    for (int i = 0; i < tamanho; i++){
        if (!strcmp(fila, palavras_chave[i])){
            return 1;
        }
    }
    return 0;
}

int isOperador(char c) {
    int tamanho = sizeof(operadores)/sizeof(operadores[0]);
    for (size_t i = 0; i < tamanho; i++) {
        if (c == operadores[i]){
            return 1;
        }
    }
    return 0;
}

int isOperadorDuplo(const char *token) {
    if (strlen(token) < 2) {
        return 0;
    }

    int num_operadores = sizeof(operadores_duplos) / sizeof(operadores_duplos[0]);
    
    for (int i = 0; i < num_operadores; i++) {
        if (strncmp(token, operadores_duplos[i], 2) == 0) {
            return 1; 
        }
    }
    return 0;
}