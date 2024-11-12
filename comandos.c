#include "comandos.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

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
    } else {
        printf("Comando desconocido: %s\n", command);
    }
}


void msg_command(char *string) {
    if (string != NULL) {
        printf("%s\n", string);
    } else {
        printf("\n");
    }
}

// Funcion para crear un directorio
void creardir_command(char *string) {
    if (mkdir(string, 0777) == 0) {
        printf("Directorio '%s' creado\n", string);
    } else {
        perror("Error al crear el directorio");
    }
}

// Funcion para ejecutar comandos fuera de la shell
void out_command(char *string) {
    if (system(string) == -1) {
        perror("Error al ejecutar el comando externo");
    }
}

// Funcion para copiar un archivo
int copiar(const char *origen, const char *destino) {
    FILE *archivoOrigen = fopen(origen, "rb");
    if (!archivoOrigen) {
        perror("Error al abrir el archivo de origen para copiar");
        return -1;
    }

    FILE *archivoDestino = fopen(destino, "wb");
    if (!archivoDestino) {
        perror("Error al crear el archivo de destino para copiar");
        fclose(archivoOrigen);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, archivoOrigen)) > 0) {
        fwrite(buffer, 1, bytes, archivoDestino);
    }

    fclose(archivoOrigen);
    fclose(archivoDestino);

    return 0;
}

// Funcion para mover un archivo
int mover(const char *origen, const char *destino) {
    if (copiar(origen, destino) == 0) {
        if (remove(origen) == 0) {
            return 0; 
        } else {
            perror("Error al eliminar el archivo original después de copiar");
            return -1;
        }
    }
    return -1;
}

// Funcion para renombrar un archivo
int renombrar(const char *nombreActual, const char *nuevoNombre) {
    if (rename(nombreActual, nuevoNombre) == 0) {
        return 0; 
    } else {
        perror("Error al renombrar el archivo");
        return -1;
    }
}