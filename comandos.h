#ifndef comandos_h
#define comandos_h 
#include <sys/types.h>
void command_manager(char *string);
void msg_command(char *string);
void creardir_command(char *string);
void out_command(char *string);
void ir_command(char *path);
int copiar(const char *origen, const char *destino);
int mover(char *origen, char *destino);
int renombrar(const char *nombreActual, const char *nuevoNombre);
void listar(const char *path);
void permisos(char *archivos, mode_t permisos);
void propietario(char *file, char *user_or_group);
void contrasena(char *usuario, char *password);
void servicio(char *action, char *service_path);
void tftp (char *ftp_server, char *user, char *password, char *remote_file, char *local_path);
void usuario(char *user, char *info);
#endif