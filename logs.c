void obtener_fecha_hora(char *buffer, size_t size) {
    
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y/%m/%d %H:%M:%S", tm_info);

}
void history_log (char *linea){
    char fecha_hora[25];
    char *path = "/var/log/shell/shell_history";
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

