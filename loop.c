#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "command_manager.c"
#include "commands.c"

void main (){
    char *line = NULL; // linea de entrada de comando
    size_t len = 0; // tamaño del buffer
    ssize_t nread; // numero de caracteres leidos

    chdir("home");

    while (1){
        printf("shell>home: ");  
        nread = getline(&line, &len, stdin); //input de usuario
        if (nread == -1){
            break;
        }

        line[strcspn(line, "\n")] = 0; //se remueve salto de linea para su evaluacion en command_manager
        command_manager(line); // se llama a command_manager que tomara la linea y evaluara el comando y los argumentos

    }
    free(line);
    
}