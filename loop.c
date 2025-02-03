#include "command_manager.c"
#include "commands.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>


void main (){
    char *line = NULL; 
    size_t len = 0;
    ssize_t nread;

    //variables para la lectura de la linea de comando
    
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid); 
    if (pw == NULL) {
        perror("Error obteniendo nombre de usuario");
        return; 
    }
    //se trata de conseguir el nombre de usuario, para redireccionarlo futuramente a /home/user

    char user_home[512];
    snprintf(user_home, sizeof(user_home), "/home/%s", pw->pw_name);
    chdir(user_home);

    //aqui se hace lo explicado arriba
	
	system("export PATH=$PATH:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin");
	
	//se carga manualmente el path de cada usuario 
    login_log(1);
    while (1){
        char cwd[512];
        getcwd(cwd, sizeof(cwd));
        printf("shell:%s->%s: ", pw->pw_name, cwd); 
        nread = getline(&line, &len, stdin);

        if (nread == -1){
            login_log(0);
            break;
        }

        line[strcspn(line, "\n")] = 0;
        command_manager(line);
        history_log(line);
        if (strcmp(line, "salir") == 0){
            break;
        }
    }
    free(line);
    login_log(0);
    /*este codigo de bloque se encarga de manejar el input del usuario, aparte de mostrar informacion como el usuario en el cual se ha logeado, y
    el path absoluto desde /home. La linea de comando se pasa a command_manager, quien se encarga de el output y accion de la entrada del
    usuario*/
    
}