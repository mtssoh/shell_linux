void msg_command(char *string) {
    if (string != NULL) {
        printf("%s\n", string);
    } else {
        printf("\n");
    }
}
//clon de echo, imprime el string pasado
void ir_command(char *path) {
    if (path == NULL) {
        printf("Uso: ir <directorio>\n");
        return;
    }

    // Cambiar al directorio especificado
    if (chdir(path) == 0) {
        //con chdir() se cambia de directorio
        char cwd[MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            //con getcwd se consigue el directorio actual para imprimirlo como mensaje
            printf("Path: %s\n", cwd);
        } else {
            perror("Error al obtener el directorio actual");
        }
    } else {
        perror("Error al cambiar el directorio");
    }
}

// Funcion para crear un directorio
void creardir_command(char *string) {
    if (mkdir(string, 0777) == 0) {
        //se crea un directorio con permisos 0777 (misma notacion que chmod)
        printf("Directorio '%s' creado\n", string);
    } else {
        perror("Error al crear el directorio");
    }
}

// Funcion para ejecutar comandos fuera de la shell
void out_command(char *string) {
    if (system(string) == -1) {
        //con system, se ejecuta el comando desde otra terminal, con la shell por default del sistema
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
    //se abre el archivo origen como lectura
    FILE *archivoDestino = fopen(destino, "wb");
    if (!archivoDestino) {
        perror("Error al crear el archivo de destino para copiar");
        fclose(archivoOrigen);
        return -1;
    }
    //se abre el archivo destino como escritura

    char buffer[BUFFER_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, archivoOrigen)) > 0) {
        fwrite(buffer, 1, bytes, archivoDestino);
    }
    //se copia el contenido del archivo origen a destino

    fclose(archivoOrigen);
    fclose(archivoDestino);
    //se cierran ambos archivos

    return 0;
    
}


int mover(char *origen, char *destino) {

    char rutaDestino[BUFFER_SIZE];
    snprintf(rutaDestino, sizeof(rutaDestino), "%s/%s", destino, basename(origen));
    //se concatena el path destino desde el que se encuentra el usuario

    if (copiar(origen, rutaDestino) == 0) {
        //se copia el archivo a rutadestino
        if (remove(origen) == 0) {
            //se elimina, porque se movio
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
        //Se cambia el nombre del archivo con rename()
        return 0; 
    } else {
        perror("Error al renombrar el archivo");
        return -1;
    }
}
void listar(const char *path) {
    DIR *dir = opendir(path); 
    // Abre el directorio especificado
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignora los directorios "." y ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name); // Imprime el nombre de cada archivo/subdirectorio
        }
    }

    closedir(dir); // Cierra el directorio después de leerlo
}

void permisos(char *archivos, mode_t permisos_archivo){ //cambia permisos de archivos con argumentos <archivo> <permisos (octal)>
    char *file = strtok(archivos, " ");
    while (file != NULL){
        if (chmod(file, permisos_archivo) == -1){
            //con chmod se cambian los permisos de los archivos 
            printf("Error al cambiar permisos de %s\n", file);
        }
        else{
            printf("Permisos de %s cambiados a %o\n", file, permisos_archivo);
        }   

        file = strtok(NULL, " ");
    }
}

void propietario(char *sfile, char *user_or_group){
    //se utiliza una flag, una para cambiar el grupo y otra el propietario
    char *flag = strtok(sfile, " ");
    
    if (flag == NULL) {
        perror("Error: No se proporcionó el flag");
        return;
    }

    if (strcmp(flag, "u") == 0) {
        struct passwd *pwd = getpwnam(user_or_group);
        if (pwd == NULL) {
            perror("Error obteniendo UID del propietario");
            return;
        }
        //se obtiene el uid del propietario

        char *file = strtok(NULL, " ");
        while (file != NULL) {
            if (chown(file, pwd->pw_uid, -1) == -1) {
                //con esto, se cambia el propietario con la uid conseguida
                perror("Error al cambiar propietario");
            } else {
                printf("Propietario de %s cambiado a %s\n", file, user_or_group);
            }
            file = strtok(NULL, " ");
        }

    } else if (strcmp(flag, "g") == 0) {
        struct group *grp = getgrnam(user_or_group);
        if (grp == NULL) {
            perror("Error obteniendo GID del grupo");
            return;
        }
        //se obtiene la gid del grupo

        char *file = strtok(NULL, " ");
        while (file != NULL) {
            if (chown(file, -1, grp->gr_gid) == -1) {
                //se cambia el grupo del archivo
                perror("Error al cambiar grupo");
            } else {
                printf("Grupo de %s cambiado a %s\n", file, user_or_group);
            }
            file = strtok(NULL, " ");
        }

    } else {
        perror("Flags: u para propietario (usuario), g para cambiar grupo");
    }
    //cabe aclarar que esta funcion puede cambiar propietario y grupo de varios archivos en una linea de comando
}

void contraseña(char *usuario, char *password){
    char comando[256];
    snprintf(comando, sizeof(comando), "echo \"%s:%s\" | sudo chpasswd", usuario, password);
    //se hace una llamada al sistema de echo <usuario:password> | sudo chpasswd, esto pasa el string usuario:passwd a sudo chpasswd

    if(system(comando) == -1){
        //se ejecuta la llamada al sistema
        perror("Error al cambiar contraseña");
    } 
    else{
        printf("Contraseña cambiada correctamente para el usuario %s\n", usuario);
    }
    
}

void servicio(char* action, char *service_name){
    char service_path[256];
    char *msg = (strcmp(action, "stop") == 0 ? "apagado" : "iniciado");

    snprintf(service_path, sizeof(service_path), "/etc/init.d/%s", service_name);
    //se concatena el path de /etc/init.d con el nombre del servicio 
    if (access(service_path, X_OK) == -1){
        perror("Error: no se puede encontrar o ejecutar el script de servicio");
        return;
    }

    pid_t pid = fork();
    //se ejecuta el servicio y luego se revisa su estado
    if (pid == 0) { 
        execl(service_path, service_path, action, (char *)NULL);
        perror("Error al iniciar servidor");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) { perror("Error al crear proceso");}

    else { printf("Proceso %s %s correctamente\n", service_name, msg); }
    //se inicia un sercvicio con servicio start <servicio>
    //se apaga un servicio con servicio stop <servicio>
}

void tftp(char *ftp_server, char *user, char *password, char *remote_file, char *local_path) {
    char command[512];
    char log_path[512];

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        perror("Error obteniendo nombre de usuario");
        return;
    }

    snprintf(log_path, sizeof(log_path), "/home/%s/Shell_transferencias", pw->pw_name);

    //se consigue el nombre de usuario para guardar los logs de transferencias
    
    FILE *log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("Error abriendo el archivo de log");
        return;
    }
    
    fprintf(log_file, "Servidor: %s. Usuario: %s. Archivo remoto: %s Ruta local: %s.", ftp_server, user, remote_file, local_path);
    
    
    snprintf(command, sizeof(command),
             "echo -e \"user %s %s \\nget %s %s\\nbye\" | ftp -n %s",
             user, password, remote_file, local_path, ftp_server);

    printf("Ejecutando: %s\n", command);
    //se realiza la transferencia ftp con una llamada al sistema de ftp
   
    int status = system(command);
    if (status == -1) {
        perror("Error ejecutando el comando tftp");
        fprintf(log_file, "No exitoso.\n");
    } else {
        printf("Transferencia ftp completada\n");
        fprintf(log_file, "Exitoso, \n");
        
    }
    fclose(log_file);
}

void usuario(char *user, char *info){
    char comando[256];
    snprintf(comando, sizeof(comando), "sudo useradd -m -s /bin/shell %s -c %s", user, info); 
    system(comando);
    //se crea el usuario con esta shell /bin/shell
    //se agrega info en el campo


    printf("Usuario %s creado exitosamente\n", user);
}








    