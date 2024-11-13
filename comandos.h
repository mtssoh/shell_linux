#ifndef comandos_h
#define comandos_h 

void command_manager(char *string);
void msg_command(char *string);
void creardir_command(char *string);
void out_command(char *string);
void ir_command(char *path);
int copiar(const char *origen, const char *destino);
int mover(const char *origen, const char *destino);
int renombrar(const char *nombreActual, const char *nuevoNombre);
void listar(const char *path);
#endif