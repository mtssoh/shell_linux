void msg_command(char *string) {
    if (string != NULL) {
        printf("%s\n", string);
    } else {
        printf("\n");
    }
}
void ir_command(char *path) {
    if (path == NULL) {
        printf("Uso: ir <directorio>\n");
        return;
    }

    // Cambiar al directorio especificado
    if (chdir(path) == 0) {
        char cwd[MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
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


int mover(char *origen, char *destino) {

    char rutaDestino[BUFFER_SIZE];
    snprintf(rutaDestino, sizeof(rutaDestino), "%s/%s", destino, basename(origen));

    if (copiar(origen, rutaDestino) == 0) {
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
void listar(const char *path) {
    DIR *dir = opendir(path); // Abre el directorio especificado
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
            printf("Error al cambiar permisos de %s\n", file);
        }
        else{
            printf("Permisos de %s cambiados a %o\n", file, permisos_archivo);
        }   

        file = strtok(NULL, " ");
    }
}

void propietario(char *sfile, char *user_or_group){
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

        char *file = strtok(NULL, " ");
        while (file != NULL) {
            if (chown(file, pwd->pw_uid, -1) == -1) {
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

        char *file = strtok(NULL, " ");
        while (file != NULL) {
            if (chown(file, -1, grp->gr_gid) == -1) {
                perror("Error al cambiar grupo");
            } else {
                printf("Grupo de %s cambiado a %s\n", file, user_or_group);
            }
            file = strtok(NULL, " ");
        }

    } else {
        perror("Flags: u para propietario (usuario), g para cambiar grupo");
    }
}

void contraseña(char *usuario, char *password){
    char comando[256];
    snprintf(comando, sizeof(comando), "echo \"%s:%s\" | sudo chpasswd", usuario, password);

    if(system(comando) == -1){
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

    if (access(service_path, X_OK) == -1){
        perror("Error: no se puede encontrar o ejecutar el script de servicio");
        return;
    }

    pid_t pid = fork();

    if (pid == 0) { 
        execl(service_path, service_path, action, (char *)NULL);
        perror("Error al iniciar servidor");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) { perror("Error al crear proceso");}

    else { printf("Proceso %s %s correctamente\n", service_name, msg); }
}








    