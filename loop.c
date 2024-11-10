#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "comandos.c"

void main (){
    char *line = NULL; // linea de entrada de comando
    size_t len = 0; // tamaño del buffer
    ssize_t nread; // numero de caracteres leidos

    while (1){
        printf("shell> ");   
        nread = getline(&line, &len, stdin);
        if (nread == -1){
            break;
        }

        line[strcspn(line, "\n")] = 0;
        command_manager(line);

    }
    free(line);
    
}