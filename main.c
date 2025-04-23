#include "lexico.h"

int main (int argc, char *argv[]){

    FILE *codigo = fopen(argv[1], "r");
    if (!codigo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    analisador_lexico(codigo);
    return 0;
}