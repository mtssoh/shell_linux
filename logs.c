#include <netdb.h>
#include <arpa/inet.h>

void obtener_fecha_hora(char *buffer, size_t size) {
    
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y/%m/%d %H:%M:%S", tm_info);

}
void get_user_ip(char *ip, size_t size){
    struct addrinfo hints, *res;
    char hostname[128];
    gethostname(hostname, sizeof(hostname));

    // Configurar los hints para obtener una dirección IP (IPv4)
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // Solo IPv4
    hints.ai_socktype = SOCK_STREAM;

    // Obtener la información de la dirección IP
    int ret = getaddrinfo(hostname, NULL, &hints, &res);
    if (ret != 0 || res == NULL) {
        snprintf(ip, size, "IP desconocida");
        return;
    }

    // Convertir la dirección IP a una cadena
    struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &addr_in->sin_addr, ip, size);

    // Liberar la memoria de la estructura addrinfo
    freeaddrinfo(res);
}

void history_log (char *linea){
    char fecha_hora[25];
    char *path = "/var/log/shell/shell_history.log";
    char *user = getenv("USER");
    if (user == NULL){
        printf("No se puede obtener user");
        return;
    }

    obtener_fecha_hora(fecha_hora, sizeof(fecha_hora));
    FILE *file = fopen(path, "a");
    
    if (file == NULL){
        perror("Error al abrir archivo de log");
        return;
    }
    fprintf(file, "%s %s %s\n", linea, user, fecha_hora);
    fclose(file);

}

void error_log(char *linea){
    char *path = "/var/log/shell/shell_error.log";
    char *user = getenv("USER");

    FILE *file = fopen(path, "a");

    if (file == NULL){
        perror("Error al abrir archivo de error log");
        return;
    }

    fprintf(file, "Return message: %s. User: %s\n", linea, user);
    fclose(file);
}

void report (char *linea){
    perror(linea);
    error_log(linea);
}

void login_log(int n){
    char *user = getenv("USER");
    char *path = "/var/log/shell/shell_login.log";
    char ip[64];
    char time[25];
    obtener_fecha_hora(time, sizeof(time));
    get_user_ip(ip, sizeof(ip));
    FILE *file = fopen(path, "a");
    if (n){
        fprintf(file, "User: %s. Direccion: %s. Login: %s\n", user, ip, time);
    }
    else{
        fprintf(file, "User: %s. Logout: %s\n", user, time);
    }
    fclose(file);
}

