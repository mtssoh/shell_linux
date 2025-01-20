#include "command_manager.c"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


void main (){
    char *line = NULL; // linea de entrada de comando
    size_t len = 0; // tamaño del buffer
    ssize_t nread; // numero de caracteres leidos

    
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        perror("Error obteniendo nombre de usuario");
        return; 
    }

    char user_home[512];
    snprintf(user_home, sizeof(user_home), "/home/%s", pw->pw_name);

    chdir(user_home);
    
    while (1){
        char cwd[512];
        getcwd(cwd, sizeof(cwd));
        printf("shell:%s->%s", pw->pw_name, cwd);
        nread = getline(&line, &len, stdin); //input de usuario
        if (nread == -1){
            break;
        }

        line[strcspn(line, "\n")] = 0; //se remueve salto de linea para su evaluacion en command_manager
        command_manager(line); // se llama a command_manager que tomara la linea y evaluara el comando y los argumentos

    }
    free(line);
    
}