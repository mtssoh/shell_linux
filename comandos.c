#include "comandos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void command_manager(char *string){
   
    char *command = strtok(string, " ");
    char *argument = strtok(NULL, "");
    if (command == NULL){
        return;
    }

    if (strcmp(command, "echo") == 0){
        if (argument != NULL){
            echo(argument);
        }
       
    }
    else{
        printf("Comando desconocido: %s\n", command);
    }
    
    
}

void echo(char *string){
    if (string != NULL){
        printf("%s\n", string);
    }
    else{
        printf("\n");
    }
    return;
    
}