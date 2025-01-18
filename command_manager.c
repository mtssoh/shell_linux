
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include "comandos.h"
#include <ctype.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define MAX_PATH 1024
char path[MAX_PATH];
void clean_buffer(){
    int c;
    while ((c = getchar()) != EOF && c != '\n'){ }
}

void command_manager(char *string) {
    char *command = strtok(string, " "); // se lee hasta primer espacio (comando)
    char *argument = strtok(NULL, ""); // primer argumento
    

    if (command == NULL) {
        return;
    }

    if (strcmp(command, "msg") == 0) { // mensaje
        if (argument != NULL) {
            msg_command(argument);
        }
    } else if (strcmp(command, "creardir") == 0) { // crear directorio
        if (argument != NULL) {
            creardir_command(argument);
        }
    } else if (strcmp(command, "out") == 0) { // comando fuera de shell
        if (argument != NULL) {
            out_command(argument);
        }
    
    } else if (strcmp(command, "ir") == 0){
        if (argument != NULL){
            ir_command(argument);
        }
         
    } else if (strcmp(command, "copiar") == 0) { // copia un archivo
        char *argument1 = strtok(argument, " "); // primer argumento
        char *argument2 = strtok(NULL, ""); // segundo argumento
        printf("%s %s\n", argument, argument2);
        if (argument != NULL && argument2 != NULL) {
            if (copiar(argument1, argument2) == 0) {
                printf("Archivo copiado de %s a %s\n", argument1, argument2);
            }
        } else {
            printf("Uso: copiar <archivo_origen> <archivo_destino>\n");
        }
    } 

    else if (strcmp(command, "mover") == 0) { // Mueve un archivo
        char *argument1 = strtok(argument, " ");
        char *argument2 = strtok(NULL, "");
        if (argument != NULL && argument2 != NULL) {
            if (mover(argument1, argument2) == 0) {
                printf("Archivo movido de %s a %s\n", argument1, argument2);
            }
        } else {
            printf("Uso: mover <archivo_origen> <archivo_destino>\n");
        }
    } 

    else if (strcmp(command, "renombrar") == 0) { // Renombra un archivo
        char *argument1 = strtok(argument, " ");
        char *argument2 = strtok(NULL, "");
        if (argument1 != NULL && argument2 != NULL) {
            if (renombrar(argument1, argument2) == 0) {
                printf("Archivo renombrado de %s a %s\n", argument1, argument2);
            }
        } else {
            printf("Uso: renombrar <nombre_actual> <nuevo_nombre>\n");
        }
    }
    else if (strcmp(command, "listar") == 0) { // Lista un directorio
    if (argument != NULL) {
        listar(argument);
    } else {
        printf("Uso: listar <directorio>\n");
    }
    }
    else if (strcmp(command, "permisos") == 0){
        if (argument == NULL){
            printf("Uso: permisos <archivo(s)> <permisos>\n");
        }
        else{
            char *permisos_ch = strrchr(argument, ' ');
            if (permisos_ch == NULL){
                printf("Uso: permisos <archivo(s)> <permisos>\n");
            }
            else{
                *permisos_ch = '\0';
                char *files = argument;
                mode_t permisos_mdt = (mode_t) strtol(permisos_ch + 1, NULL, 8);

                permisos(files, permisos_mdt);
            }
        }
    }
    else if (strcmp(command, "propietario") == 0) {
        if (argument == NULL) {
            printf("Uso:\npropietario u <archivo(s)> <propietario>\npropietario g <archivo(s)> <grupo>\n");
            return;
        }
        char *user_or_group = strrchr(argument, ' ');
        *user_or_group = '\0';
        char *files = argument;
        propietario(files, user_or_group + 1);
        
    }
    else if(strcmp(command, "contraseña") == 0){
        if (argument == NULL){
            printf("Uso: contraseña <usuario> <nueva contraseña>");
        }
        printf("Estas seguro de cambiar su contraseña? Y/N: ");
        char sec;

        scanf("%c", &sec);

        clean_buffer();
        if (toupper(sec) == 'Y'){
            char *user = strtok(argument, " ");
            char *password = strtok(NULL, " ");
            contraseña(user, password);
        }
        else{
            printf("Cambio de contraseña cancelado.\n");
        }
    }
    else if (strcmp(command, "servicio") == 0){
        char *action = strtok(argument, " ");
        char *service_name = strtok(NULL, " ");
        if (action == NULL || service_name == NULL){
            printf("Uso: servicio, <accion (start, stop, restart)> <servicio>\n");
        }
        else{
            servicio(action, service_name);
        }
    }

    else {
        printf("Comando desconocido: %s\n", command);
    }
}


