#include "comandos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void command_manager(char *string){
   
    char *command = strtok(string, " "); // se lee hasta primer espacio (comando)
    char *argument = strtok(NULL, ""); // se lee hasta final del string (argumento(s))
    if (command == NULL){
        return;
    }

    if (strcmp(command, "echo") == 0){ //se compara string de comando con "echo" para su ejecucion
        if (argument != NULL){
            echo(argument);
        }
       
    }
    else{
        printf("Comando desconocido: %s\n", command);
    }
    
    
}

void echo(char *string){ //imprime argumento
    if (string != NULL){ 
        printf("%s\n", string);
    }
    else{
        printf("\n");
    }
    return;
    
}