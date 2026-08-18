# Exam Rank 6 - mini_serv: Explicacion Extensiva

## Resumen del Ejercicio

Hay que escribir un servidor de chat en C que escuche en `127.0.0.1` en un puerto dado como argumento. Los clientes se conectan via TCP, y todo mensaje que un cliente envia se retransmite a todos los demas clientes conectados, precedido por `"client %d: "` en cada linea. Es basicamente un **chat relay server** multiplexado con `select()`.

---

## Requisitos del Subject

### Argumentos y errores

| Situacion | Accion |
|-----------|--------|
| No se pasa argumento (argc != 2) | Escribir en **stderr**: `"Wrong number of arguments\n"` y salir con status 1 |
| Cualquier syscall falla antes de aceptar conexiones (socket, bind, listen) | Escribir en **stderr**: `"Fatal error\n"` y salir con status 1 |
| Fallo de malloc/calloc/realloc | Escribir en **stderr**: `"Fatal error\n"` y salir con status 1 |

### Comportamiento del servidor

1. **Escuchar solo en 127.0.0.1** (loopback), puerto indicado por `argv[1]`.
2. **No bloqueante**: el servidor nunca debe quedarse esperando un solo cliente. Se usa `select()` para multiplexar.
3. **No desconectar clientes lentos**: si un cliente no lee los mensajes que le envias, no lo desconectes.
4. **No usar `#define`** en ningun sitio del codigo.

### Identificacion de clientes

- El primer cliente que se conecta recibe **id 0**.
- Cada nuevo cliente recibe `ultimo_id + 1` (un contador global incrementado con cada conexion).
- El id **no se reutiliza** si un cliente se desconecta.

### Mensajes del servidor

| Evento | Mensaje enviado a TODOS los demas clientes |
|--------|---------------------------------------------|
| Cliente se conecta | `"server: client %d just arrived\n"` |
| Cliente se desconecta | `"server: client %d just left\n"` |

### Retransmision de mensajes

- Cuando un cliente envia un mensaje, el servidor lo reenvia a **todos los demas clientes**.
- Cada **linea** (separada por `\n`) va precedida de `"client %d: "` donde `%d` es el id del emisor.
- Un solo `recv()` puede contener multiples `\n` --> hay que separar por lineas y prefijar cada una.
- Los mensajes solo contienen caracteres imprimibles + `\n`.

### Restricciones

- **Funciones permitidas**: write, close, select, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset
- **Sin leaks** de memoria ni de file descriptors.
- Enviar los mensajes **lo mas rapido posible** (sin buffering innecesario).

---

## Arquitectura del Programa

### Flujo general

```
1. Parsear argv[1] como puerto
2. Crear socket TCP (AF_INET, SOCK_STREAM)
3. Bind a 127.0.0.1:puerto
4. Listen
5. Bucle infinito:
   a. Copiar fd_set maestro a read_set y write_set
   b. select(max_fd + 1, &read_set, &write_set, NULL, NULL)
   c. Iterar por todos los fds:
      - Si es el socket servidor y tiene lectura pendiente -> accept() nuevo cliente
      - Si es un cliente y tiene lectura pendiente -> recv() datos
        - Si recv <= 0 -> cliente desconectado -> notificar y limpiar
        - Si recv > 0 -> acumular en buffer, extraer mensajes completos, retransmitir
```

### Estructuras de datos necesarias

```c
int count = 0;            // Contador global de IDs (nunca decrece)
int max_fd = 0;           // El fd mas alto activo (necesario para select)
int ids[65536];           // ids[fd] = id asignado al cliente con ese fd
char *msgs[65536];        // msgs[fd] = buffer parcial de ese cliente (mensajes sin \n final)
fd_set all_fds;           // Set maestro de todos los fds activos
fd_set read_fds;          // Copia temporal para select (lectura)
fd_set write_fds;         // Copia temporal para select (escritura)
```

Nota: se usan arrays indexados por fd porque los fds son enteros pequenos (< 65536 con FD_SETSIZE). Es la forma mas simple aunque no la mas elegante.

---

## Funciones Auxiliares Proporcionadas

### `extract_message(char **buf, char **msg)`

Esta funcion extrae un mensaje completo (terminado en `\n`) del buffer acumulado.

**Como funciona:**
1. Busca el primer `\n` en `*buf`.
2. Si lo encuentra:
   - Crea un nuevo buffer con todo lo que hay **despues** del `\n`.
   - `*msg` apunta al mensaje extraido (incluye el `\n`).
   - `*buf` ahora apunta al resto.
   - Retorna `1`.
3. Si no hay `\n`, retorna `0` (mensaje incompleto, seguir acumulando).
4. Si falla el calloc, retorna `-1`.

**Uso tipico:**
```c
char *msg;
while (extract_message(&(msgs[fd]), &msg)) {
    // msg contiene una linea completa con \n al final
    // Enviar "client %d: " + msg a todos los demas
    free(msg);
}
```

**CUIDADO**: En `mini_serv2.c` (version de tu companero), el while empieza con `(*buf)[++i]` en vez de `(*buf)[i]` con `i++` al final. Esto **salta el primer caracter** y es un BUG. La version correcta es la de `main.c` donde se comprueba `(*buf)[i]` y luego se hace `i++`.

### `str_join(char *buf, char *add)`

Concatena `add` al final de `buf`, liberando el `buf` antiguo.

**Como funciona:**
1. Calcula longitud de buf (o 0 si es NULL).
2. Malloc para buf + add + 1.
3. Copia buf al nuevo, concatena add.
4. Libera el buf antiguo.
5. Retorna el nuevo buffer.

**Uso tipico:**
```c
msgs[fd] = str_join(msgs[fd], buf_read);
// Acumula lo recibido al buffer parcial del cliente
```

---

## Desglose Paso a Paso de la Implementacion

### 1. Validacion de argumentos

```c
if (ac != 2) {
    write(2, "Wrong number of arguments\n", 26);
    exit(1);
}
```

Importante: el mensaje va a **stderr** (fd 2), no a stdout.

### 2. Creacion del socket

```c
FD_ZERO(&all_fds);
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
    fatal_error();
max_fd = sockfd;
FD_SET(sockfd, &all_fds);
```

- `AF_INET` = IPv4
- `SOCK_STREAM` = TCP
- Se anade el socket del servidor al fd_set maestro.

### 3. Bind

```c
struct sockaddr_in servaddr;
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1 en decimal
servaddr.sin_port = htons(atoi(av[1]));

if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    fatal_error();
```

- `htonl(2130706433)` = `htonl(0x7F000001)` = 127.0.0.1 en network byte order.
- `htons()` convierte el puerto a network byte order.

### 4. Listen

```c
if (listen(sockfd, 128) != 0)
    fatal_error();
```

El backlog (128) indica cuantas conexiones pendientes puede encolar el kernel.

### 5. Bucle principal con select()

```c
while (1) {
    read_fds = write_fds = all_fds;  // Copiar set maestro
    if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) < 0)
        fatal_error();
    // Iterar fds...
}
```

**Por que copiar?** `select()` modifica los fd_sets para indicar cuales estan listos. Si no copias, pierdes el registro de que fds monitorizas.

**write_fds**: se usa para saber que clientes estan listos para recibir datos (puedes enviarles sin bloquear).

### 6. Aceptar nuevos clientes

```c
if (fd == sockfd) {
    socklen_t addr_len = sizeof(servaddr);
    int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &addr_len);
    if (client_fd >= 0) {
        // Registrar cliente
        if (client_fd > max_fd)
            max_fd = client_fd;
        ids[client_fd] = count++;
        msgs[client_fd] = NULL;
        FD_SET(client_fd, &all_fds);
        // Notificar a todos
        sprintf(buf_write, "server: client %d just arrived\n", ids[client_fd]);
        notify_other(client_fd, buf_write);
        break;  // Salir del for, volver a select
    }
}
```

El `break` despues de accept es para volver a llamar a `select()` inmediatamente, ya que el fd_set puede haber cambiado.

### 7. Recibir mensajes de clientes

```c
int bytes_read = recv(fd, buf_read, 1000, 0);
if (bytes_read <= 0) {
    // Cliente desconectado
    sprintf(buf_write, "server: client %d just left\n", ids[fd]);
    notify_other(fd, buf_write);
    free(msgs[fd]);
    FD_CLR(fd, &all_fds);
    close(fd);
    break;
}
buf_read[bytes_read] = '\0';
msgs[fd] = str_join(msgs[fd], buf_read);
send_msg(fd);
```

- `recv` retorna 0 cuando el cliente cierra la conexion (EOF).
- Se acumulan los bytes en `msgs[fd]` y se extraen mensajes completos.

### 8. Enviar mensajes a otros clientes

```c
void notify_other(int author, char *str) {
    for (int fd = 0; fd <= max_fd; fd++)
        if (FD_ISSET(fd, &write_fds) && fd != author)
            send(fd, str, strlen(str), 0);
}

void send_msg(int fd) {
    char *msg;
    while (extract_message(&(msgs[fd]), &msg)) {
        sprintf(buf_write, "client %d: ", ids[fd]);
        notify_other(fd, buf_write);
        notify_other(fd, msg);
        free(msg);
    }
}
```

- Se envia `"client %d: "` + el contenido de la linea (que ya incluye `\n`) a todos excepto al emisor.
- Solo se envia a fds que estan en `write_fds` (listos para escritura segun select).

---

## Diferencias entre main.c y mini_serv2.c

| Aspecto | main.c (starter) | mini_serv2.c (solucion companero) |
|---------|-------------------|-----------------------------------|
| Proposito | Codigo inicial proporcionado por 42 | Implementacion completa |
| select() | No lo usa | Si, bucle con select() |
| Multiples clientes | No (solo hace un accept) | Si |
| extract_message | Version correcta (`(*buf)[i]` con `i++`) | **BUG**: usa `(*buf)[++i]` (salta primer char) |
| Error handling | printf + exit(0) | write(2, ...) + exit(1) |
| Funciones prohibidas | Usa printf, bzero (bzero esta permitida) | No usa printf |

---

## Errores Comunes a Evitar

1. **Usar printf**: No esta en la lista de funciones permitidas. Usar `sprintf` + `send`/`write`.
2. **Olvidar actualizar max_fd**: select necesita `max_fd + 1` como primer argumento.
3. **No hacer break despues de accept o remove**: Si modificas el fd_set durante la iteracion y no haces break, puedes iterar sobre fds invalidos.
4. **Buffer demasiado pequeno para sprintf**: `"server: client %d just arrived\n"` puede ser largo si el id tiene muchos digitos. Un buffer de 42 bytes es suficiente para los casos del examen pero es justo.
5. **Leak en msgs[fd]**: Al desconectar un cliente, hay que hacer `free(msgs[fd])` antes de eliminarlo.
6. **No cerrar el fd del cliente**: Leak de file descriptors.
7. **Enviar al socket del servidor**: El socket de escucha esta en `all_fds` pero no debes enviarle mensajes. La condicion `fd != author` no es suficiente; hay que asegurar que no envias al sockfd. En la practica funciona porque el sockfd no suele estar en write_fds como destino valido, pero cuidado.
8. **Bug del extract_message con `++i`**: La version de tu companero salta el primer caracter de cada mensaje. Usa la version de main.c.

---

## Como Testear

### Compilar

```bash
cc -Wall -Wextra -Werror mini_serv.c -o mini_serv
```

### Ejecutar el servidor

```bash
./mini_serv 8080
```

### Conectar clientes (en terminales separadas)

```bash
nc 127.0.0.1 8080
```

### Test basico

1. Abrir terminal 1: `./mini_serv 4242`
2. Abrir terminal 2: `nc 127.0.0.1 4242` --> En terminal 3 aparece: `server: client 0 just arrived`
3. Abrir terminal 3: `nc 127.0.0.1 4242` --> En terminal 2 aparece: `server: client 1 just arrived`
4. En terminal 2 escribir "hola" + Enter --> En terminal 3 aparece: `client 0: hola`
5. Cerrar terminal 3 (Ctrl+C o Ctrl+D) --> En terminal 2 aparece: `server: client 1 just left`

### Test con multiples lineas

Enviar un mensaje con varias lineas de golpe:
```bash
printf "linea1\nlinea2\nlinea3\n" | nc 127.0.0.1 4242
```
Debe aparecer en los otros clientes:
```
client 0: linea1
client 0: linea2
client 0: linea3
```

---

## Pseudocodigo Final Completo

```
PROGRAMA mini_serv:
    SI argc != 2:
        stderr("Wrong number of arguments\n")
        exit(1)

    FD_ZERO(all_fds)
    sockfd = socket(AF_INET, SOCK_STREAM, 0)
    SI error: fatal_error()
    max_fd = sockfd
    FD_SET(sockfd, all_fds)

    configurar servaddr con 127.0.0.1 y puerto argv[1]
    bind(sockfd, servaddr)
    SI error: fatal_error()
    listen(sockfd, 128)
    SI error: fatal_error()

    BUCLE INFINITO:
        read_fds = write_fds = all_fds
        select(max_fd + 1, &read_fds, &write_fds, NULL, NULL)
        SI error: fatal_error()

        PARA fd = 0 HASTA max_fd:
            SI fd NO esta en read_fds: continuar

            SI fd == sockfd:
                client_fd = accept(sockfd, ...)
                SI client_fd >= 0:
                    actualizar max_fd si necesario
                    ids[client_fd] = count++
                    msgs[client_fd] = NULL
                    FD_SET(client_fd, all_fds)
                    enviar "server: client %d just arrived\n" a todos los demas
                    break

            SINO:
                bytes = recv(fd, buffer, 1000, 0)
                SI bytes <= 0:
                    enviar "server: client %d just left\n" a todos los demas
                    free(msgs[fd])
                    FD_CLR(fd, all_fds)
                    close(fd)
                    break
                SINO:
                    buffer[bytes] = '\0'
                    msgs[fd] = str_join(msgs[fd], buffer)
                    MIENTRAS extract_message(&msgs[fd], &msg):
                        enviar "client %d: " + msg a todos los demas
                        free(msg)
```

---

## Resumen para el Examen

Lo que tienes que saber escribir de memoria:

1. **Funciones dadas** (extract_message y str_join): Copiarlas del main.c sin errores.
2. **Setup del socket**: socket -> bind -> listen, con manejo de errores.
3. **Bucle select**: copiar fd_sets, llamar select, iterar.
4. **Manejo de conexion**: accept + registrar id + notificar.
5. **Manejo de mensaje**: recv + acumular + extract + broadcast.
6. **Manejo de desconexion**: notificar + free + FD_CLR + close.

El codigo completo son unas 120-160 lineas. Practica hasta que puedas escribirlo en menos de una hora sin consultar nada.
