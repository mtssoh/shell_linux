#include "comandos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void command_manager(char *string){
   
    char *command = strtok(string, " "); // se lee hasta primer espacio (comando)
    char *argument = strtok(NULL, ""); // se lee hasta final del string (argumento(s))
    if (command == NULL){
        return;
    }

    if (strcmp(command, "msg") == 0){ //se compara string de comando con "echo" para su ejecucion
        if (argument != NULL){
            msg_command(argument);
        }
       
    }
    else if (strcmp(command, "creardir") == 0){ // crear directorio
        if (argument != NULL){
            creardir_command(argument);
        }
    }
    else if (strcmp(command, "out") == 0){//comando fuera de shell
        if (argument != NULL){
            out_command(argument);
        }
    }
    else{
        printf("Comando desconocido: %s\n", command);
    }
    
    
}

void msg_command(char *string){ //imprime argumento
    if (string != NULL){ 
        printf("%s\n", string);
    }
    else{
        printf("\n");
    }
    return;
    
}
void creardir_command(char *string){ // funcion que crea directorio con nombre del argumento
    if (mkdir(string, 0777) == 0){
        printf("Directorio %s creado\n", string);
    }
    else{
        perror("Error al crear directorio");
    }

}
void out_command(char *string){ // funcion para ejecutar comandos fuera de la shell, la nomenclatura debe ser out <comando> <argumento(s)>
    if(system(string) == -1){
        perror("Error al ejecutar el comando");
    }
}