-----------------------------------------------------------------------------------------------------
 10/11/2024	Se agrego loop principal de la linea de comando, y funcion msg (echo)                    | Testeado (funciona)
        	Se agrego comando de creardir (llamada al sistema de mkdir)                              |
       	 	Se agrego comando out para comandos fuera de la shell (out <comando + argunmento>)       |
-----------------------------------------------------------------------------------------------------
11/11/2024 	Se agrego comandos de mover (funcionamiento no terminado), copiar y renombrar, aparte    | Testeado (funcionando)
        	Se arreglo un bug del comando out con la nueva implementacion de los comandos.           |
-----------------------------------------------------------------------------------------------------
12/11/2024 	Se agrego comando ir (comando cd, funcionamiento probablemente no terminado debido a     | Testeado (funciona)
			que no estoy seguro si hace syscall a cd o no)                                           |
-----------------------------------------------------------------------------------------------------
14/1/2025	Se agrego comando permisos, se debe probar su funcionalidad en sistemas de archivos		 | Testeado (funciona) 
			ext4, pero deberia de andar bien.		                                                 |
-----------------------------------------------------------------------------------------------------
15/1/2025	Se agrego propietario, debe progarse con grupos sobre el lfs.                            | Pendiente
-----------------------------------------------------------------------------------------------------
16/1/2025	Se agrego contraseña, funciona bien, no se necesita mas testeo.                          | Testeado (funciona)
-----------------------------------------------------------------------------------------------------
18/1/2025	Se agrego servicio, funciona bien con procesos sin funcionalidad, quizas no requiera mas | Testeado (funciona)
			testeo.																					 |
-----------------------------------------------------------------------------------------------------
20/01/2025	Se agrego tftp, comando que realiza una transaccion ftp, funciona bien probado con       | Testeado (funciona)
			ftp.gnu.org anonymous mail@mail.com README path_local									 | 
-----------------------------------------------------------------------------------------------------
21/01/2025	Se agrego usuario, añade un usuario, tambien se mejoro la terminal de usuario, mostrando | Testeado (funciona)
			mas informacion. Se añadio un sistema de logs de comandos realizados por los usuarios.   |
-----------------------------------------------------------------------------------------------------



Pendientes: 
1. Registrar inicios de sesion y salida de sesion del usuario (detalles en classroom). (IMPLEMENTAR Y PROBAR) - (RESUELTO)
2. Registrar todos los comandos realizados por cada usuario con timestamps. (RESUELTO)
3. Registrar errores de retorno en un log. (PENDIENTE) - (RESUELTO)
4. Documentar codigo.(RESUELTO)
5. Transferencia FTP o SCP. (No se como hacer esto) - (RESUELTO)
6. Descargar dependencias necesarias en el lfs. (RESUELTO) - (probar de vuelta)
7. Agregar usuario. (RESUELTO)