# mini_serv2.c — Explicacion linea a linea

(extract_message y str_join se copian tal cual de main.c, no se explican aqui)

---

## Variables globales (linea 56-58)

```c
int count = 0, max_fd = 0, ids[65536];
```
- `count` — contador incremental para asignar IDs a clientes nuevos
- `max_fd` — el fd mas alto activo (select necesita saber hasta donde mirar)
- `ids[65536]` — mapa fd -> ID de cliente. Si fd=5, `ids[5]` es su ID

```c
char *msgs[65536], buf_read[1001], buf_write[42];
```
- `msgs[fd]` — buffer acumulado de lo que cada cliente ha enviado pero aun no tiene `\n`
- `buf_read` — buffer temporal donde `recv` deposita los bytes leidos
- `buf_write` — buffer temporal para construir mensajes con `sprintf`

```c
fd_set read_fds, write_fds, all_fds;
```
- `all_fds` — set "maestro" de todos los fds activos (servidor + clientes)
- `read_fds` — copia que select modifica para indicar quien tiene datos listos para leer
- `write_fds` — copia que select modifica para indicar quien puede recibir escritura (la usamos como "lista de conectados" en `notify_other`)

---

## `fatal_error` (lineas 60-64)

```c
write(2, "Fatal error\n", 12);  // escribe en stderr (fd 2)
exit(1);                         // sale con codigo de error
```

---

## `notify_other` (lineas 66-71)

Envia un string a todos los clientes conectados excepto `author`.

```c
for (int fd = 0; fd <= max_fd; fd++)
    if (FD_ISSET(fd, &write_fds) && fd != author)
        send(fd, str, strlen(str), 0);
```
- `write_fds` es una copia de `all_fds` hecha antes de select
- Asi sabe que fds estan conectados (incluye servidor + clientes)
- El servidor nunca recibe mensajes por send, asi que no importa que este incluido

---

## `send_msg` (lineas 73-83)

Extrae lineas completas del buffer del cliente y las retransmite a todos los demas.

```c
char *msg;
while(extract_message(&(msgs[fd]), &msg))  // mientras haya lineas completas
{
    sprintf(buf_write, "client %d: ", ids[fd]);  // construye prefijo
    notify_other(fd, buf_write);                  // envia prefijo a todos menos al autor
    notify_other(fd, msg);                        // envia el cuerpo del mensaje
    free(msg);                                    // libera la linea extraida
}
```

Envia prefijo y cuerpo por separado (dos sends). Asi evita concatenarlos en un solo string.

---

## `main` — setup (lineas 85-103)

```c
if (ac != 2)
    write(2, "Wrong number of arguments\n", 26), exit(1);
```
Valida argumentos. Usa el operador coma `,` para meter write + exit en una sola sentencia.

```c
FD_ZERO(&all_fds);                                    // limpia el set maestro
max_fd = socket(AF_INET, SOCK_STREAM, 0);             // crea socket TCP, guarda fd en max_fd
if (max_fd < 0) fatal_error();                        // verifica
FD_SET(max_fd, &all_fds);                             // lo anade al set maestro
int sockfd = max_fd;                                  // guarda referencia al socket servidor
```

```c
struct sockaddr_in servaddr;
bzero(&servaddr, sizeof(servaddr));                   // pone todo a 0
servaddr.sin_family = AF_INET;                        // IPv4
servaddr.sin_addr.s_addr = htonl(2130706433);         // 127.0.0.1 en network byte order
servaddr.sin_port = htons(atoi(av[1]));               // puerto del argumento
if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) || listen(sockfd, 128))
    fatal_error();
```
- `bind` asigna la direccion IP:puerto al socket
- `listen` pone el socket en modo escucha con cola de 128 conexiones pendientes
- Se combinan con `||` para ahorrar lineas (si bind falla, ni llega a listen)

---

## `main` — loop principal (lineas 104-145)

```c
while (1)
{
    read_fds = write_fds = all_fds;  // copia el set maestro a ambos
    if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) < 0)
        fatal_error();
```
- `select` bloquea hasta que algun fd tenga actividad
- Modifica `read_fds`: solo quedan los fds con datos pendientes de lectura
- Modifica `write_fds`: solo quedan los fds listos para escritura (casi siempre todos)

```c
    for (int fd = 0; fd <= max_fd; fd++)
        if (FD_ISSET(fd, &read_fds))    // este fd tiene datos listos
```

### Nueva conexion (fd == sockfd):
```c
            socklen_t addr_len = sizeof(servaddr);
            int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &addr_len);
            if (client_fd >= 0)                      // accept exitoso
            {
                if(client_fd > max_fd)               // actualiza fd maximo si es necesario
                    max_fd = client_fd;
                ids[client_fd] = count++;            // asigna ID incremental al nuevo fd
                msgs[client_fd] = NULL;             // sin buffer pendiente
                FD_SET(client_fd, &all_fds);        // anade al set maestro
                sprintf(buf_write, "server: client %d just arrived\n", ids[client_fd]);
                notify_other(client_fd, buf_write); // avisa a todos menos al nuevo
                break;                              // sale del for, vuelve a select
            }
```

### Cliente envia datos (else):
```c
            int bytes_read = recv(fd, buf_read, 1000, 0);  // lee hasta 1000 bytes
            if (bytes_read <= 0)                             // desconexion o error
            {
                sprintf(buf_write, "server: client %d just left\n", ids[fd]);
                notify_other(fd, buf_write);    // avisa a todos
                free(msgs[fd]);                 // libera su buffer
                FD_CLR(fd, &all_fds);           // quita del set maestro
                close(fd);                      // cierra el fd
                break;                          // sale del for, vuelve a select
            }
            buf_read[bytes_read] = '\0';        // termina el string
            msgs[fd] = str_join(msgs[fd], buf_read);  // acumula en su buffer
            send_msg(fd);                       // extrae y retransmite lineas completas
```

---

## Resumen de flujo

1. `select` espera actividad
2. Si el socket servidor tiene actividad -> `accept` nuevo cliente
3. Si un cliente tiene actividad -> `recv` datos
   - Si recv retorna 0 o negativo -> cliente desconectado, notificar y limpiar
   - Si recv retorna datos -> acumular en buffer, extraer lineas completas, retransmitir
4. `break` despues de cada evento para reiniciar desde select (porque `all_fds` cambio)
