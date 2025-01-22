
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
#include <libgen.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define MAX_PATH 1024
char path[MAX_PATH];
void clean_buffer(){
    int c;
    while ((c = getchar()) != EOF && c != '\n'){ }
}
//se usa para limpiar el buffer, para poder usar scanf() sin problemas

void command_manager(char *string) {
    char *command = strtok(string, " "); // se lee hasta primer espacio (comando)
    char *argument = strtok(NULL, ""); // argumento(s) del comando
    

    if (command == NULL) {
        return;
    }
    //si no se entro nada, no se hace nada

    if (strcmp(command, "msg") == 0) { // mensaje
        if (argument != NULL) {
            msg_command(argument);
        }
        else{
            printf("Uso: msg <mensaje>");
        }

    //llama a msg(), clon de echo


    } else if (strcmp(command, "creardir") == 0) { 
        if (argument != NULL) {
            creardir_command(argument);
        }
        else{
            printf("Uso: creardir <directorio>");
        }

        //llama a creardir(), clon de mkdir

    } else if (strcmp(command, "out") == 0) { // comando fuera de shell
        if (argument != NULL) {
            out_command(argument);
        }
        else{
            printf("Uso: out <comando + argumento>");
        }

        //llama a out, se usa para comandos fuera de la shell, uso arriba
    
    } else if (strcmp(command, "ir") == 0){
        if (argument != NULL){
            ir_command(argument);
        }
        else{
            printf("Uso: ir <path>");
        }

        //llama a ir, clon de cd
         
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

        //llama a copiar, que copia el contenido de un archivo a otro
    } 

    else if (strcmp(command, "mover") == 0) { // Mueve un archivo
        char *argument1 = strtok(argument, " ");
        char *argument2 = strtok(NULL, "");
        if (argument != NULL && argument2 != NULL) {
            if (mover(argument1, argument2) == 0) {
                printf("Archivo %s movido a %s\n", argument1, argument2);
            }
        } else {
            printf("Uso: mover <archivo_origen> <archivo_destino>\n");
        }

        //llama a mover, que copia un archivo de un lugar a otro y luego lo elimina
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

        //llama a renombrar, renombra un archivo
    }
    else if (strcmp(command, "listar") == 0) { // Lista un directorio
    if (argument != NULL) {
        listar(argument);
    } else {
        printf("Uso: listar <directorio>\n");
    }
    //llama a listar, clon de ls
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
                //todo esto es para separar el string de argumento inicial, en distintos argumentos para la funcion permisos()
                permisos(files, permisos_mdt);
            }
        }
        //llama a permisos, con este comando se puede cambiar permisos de varios archivos con la notacion octal de chmod
    }
    else if (strcmp(command, "propietario") == 0) {
        if (argument == NULL) {
            printf("Uso:\npropietario u <archivo(s)> <propietario>\npropietario g <archivo(s)> <grupo>\n");
            return;
        }
        char *user_or_group = strrchr(argument, ' ');
        *user_or_group = '\0';
        char *files = argument;
        propietario(files, user_or_group + 1); //todo esto para separar el string de argumento inicial en distintos argumentos para la funcion propietario()

        //llama a propietario, cambia el propietario de archivo(s)
        
    }
    else if(strcmp(command, "contraseña") == 0){
        if (argument == NULL){
            printf("Uso: contraseña <usuario> <nueva contraseña>");
        }
        else{
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
        
        //llamada a la funcion para cambiar la contraseña, cuenta con un mensaje de advertencia y confirmacion, para evitar errores
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
        //sistema para levantar y apagar procesos en 2do plano
    }
    else if (strcmp(command, "tftp") == 0){
        char *ftp_server = strtok(argument, " ");
        char *user = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        char *remote_file = strtok(NULL, " ");
        char *local_path = strtok(NULL, " ");

        if (ftp_server == NULL || user == NULL || password == NULL || remote_file == NULL || local_path == NULL){
            printf("Uso: tftp <servidor_ftp> <user> <password> <archivo remoto> <ruta local>\n");
        }
        else{
            tftp(ftp_server, user, password, remote_file, local_path);
        }
    }
        //funcion para realizar una transferencia ftp, basicamente se simplifica la interfaz de ftp 
        //para poner todo en una linea de comando
    else if (strcmp(command, "usuario") == 0){
        
        char *user = strtok(argument, " ");
        char *info;
        if (user != NULL){
            info = argument + strlen(user) + 1;
        }
        

        if (user == NULL || info == NULL){
            printf("Uso: usuario <user> <info>\n<");
            return;
        }

        else{
            usuario(user, info);
        }
    }
    else if (strcmp(command, "salir") == 0){
        printf("Saliendo del sistema...\n");
    }
    else {
        printf("Comando desconocido: %s\n", command);
        //comando desconocido
    }
}


