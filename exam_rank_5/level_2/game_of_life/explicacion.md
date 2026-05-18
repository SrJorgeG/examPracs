# Game of Life — Explicacion para el Examen

## Enunciado

Programa en C que simula el **Juego de la Vida de Conway**.
- **Prototipo**: `./life width height iterations`
- Lee comandos de stdin para dibujar la configuracion inicial
- Despues simula N iteraciones del Game of Life
- Imprime el tablero final

Solo se permiten: `atoi, read, putchar, malloc, calloc, realloc, free`.

---

## Conceptos Clave a Recordar

### 1. El sistema de dibujo con "lapiz"

Hay un lapiz que empieza en la esquina superior izquierda (0,0), **arriba** (no dibujando).

| Comando | Accion |
|---------|--------|
| `w` | Mover arriba (y decrece) |
| `s` | Mover abajo (y incrementa) |
| `a` | Mover izquierda (x decrece) |
| `d` | Mover derecha (x incrementa) |
| `x` | Alternar lapiz (baja ↔ sube) |

**Reglas del lapiz**:
- Empieza **arriba** (no dibuja)
- La primera `x` lo baja (empieza a dibujar)
- La segunda `x` lo sube (deja de dibujar)
- Solo pinta la celda donde esta DESPUES de moverse (si el lapiz esta bajado)

### 2. Logica de dibujo — el orden IMPORTA

```c
while (read(0, &c, 1) > 0)
{
    if (c == 'x')
        pencil = !pencil;        // Toggle del lapiz
    if (c == 'a' && x > 0)
        x--;
    else if (c == 'd' && x < width - 1)
        x++;
    else if (c == 'w' && y > 0)
        y--;
    else if (c == 's' && y < height - 1)
        y++;
    if (pencil)                   // Pinta DESPUES de mover
        board[y * width + x] = 1;
}
```

> **MUY IMPORTANTE**: El lapiz pinta la posicion ACTUAL despues de moverse. Si el lapiz esta bajado y el comando es `x`, primero se baja/sube, luego NO se mueve (porque `x` no es wasd), y luego pinta la posicion actual.
>
> Los movimientos estan limitados: no pueden salirse del tablero. Si el lapiz intenta salirse, se queda en el borde.

### 3. Representacion del tablero — Array 1D

```c
char *board = calloc(width * height, 1);      // 0 = muerta, 1 = viva
char *nextboard = calloc(width * height, 1);   // Buffer para la nueva generacion
```

> **Formula de acceso**: `board[y * width + x]` — fila `y`, columna `x`.
>
> `calloc` inicializa a cero → todas las celdas empiezan muertas.

### 4. Las reglas del Game of Life

| Celda actual | Vecinos vivos | Resultado |
|-------------|---------------|-----------|
| Viva | < 2 | Muere (soledad) |
| Viva | 2 o 3 | Sobrevive |
| Viva | > 3 | Muere (sobrepoblacion) |
| Muerta | Exactamente 3 | Nace |

```c
if (board[y * width + x])  // Si la celda esta viva
{
    if (n == 2 || n == 3)
        nextboard[y * width + x] = 1;  // Sobrevive
    else
        nextboard[y * width + x] = 0;  // Muere
}
else  // Si la celda esta muerta
{
    if (n == 3)
        nextboard[y * width + x] = 1;  // Nace
    else
        nextboard[y * width + x] = 0;  // Sigue muerta
}
```

### 5. Conteo de vecinos — Chequeo de bordes

```c
static int countneighbours(char *board, int width, int height, int x, int y)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;  // Saltar la celda misma
            int neighbour_x = x + dx;
            int neighbour_y = y + dy;
            if ((neighbour_x >= 0 && neighbour_x < width) &&
                neighbour_y >= 0 && neighbour_y < height)
            {
                count += board[neighbour_y * width + neighbour_x];
            }
        }
    }
    return (count);
}
```

> **Nota clave**: Las celdas fuera del tablero se consideran **muertas**. Si un vecino cae fuera de los limites, simplemente no se cuenta.
>
> **Truco**: Como `board` contiene 0 o 1, puedes hacer `count += board[...]` directamente. No necesitas un `if`. Si la celda esta viva (1), suma 1. Si esta muerta (0), suma 0.

### 6. Double Buffering — No modificar mientras lees

```c
for (int i = 0; i < iter; i++)
{
    step(board, nextboard, width, height);
    char *tmp = board;
    board = nextboard;
    nextboard = tmp;   // Swap de punteros, no copia de datos
}
```

> **CRITICO**: Necesitas DOS tableros. Si modificas el tablero mientras lo lees, las generaciones intermedias contaminan el calculo. El swap de punteros es O(1) — no copies datos!

### 7. Impresion del tablero

```c
static void printboard(char *board, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (board[y * width + x])
                putchar('0');
            else
                putchar(' ');
        }
        putchar('\n');
    }
}
```

> **Recuerda**: `0` (caracter cero) para viva, espacio para muerta. No confundas `'0'` con `0`.

---

## Walkthrough del primer ejemplo

```
$> echo 'sdxddssaaww' | ./a.out 5 5 0 | cat -e
     $
 000 $
 0 0 $
 000 $
     $
```

Vamos paso a paso con `sdxddssaaww` en un tablero 5x5, lapiz empieza arriba en (0,0):

| Comando | Posicion | Lapiz | Accion |
|---------|----------|-------|--------|
| `s` | (0,1) | up | Mueve abajo |
| `d` | (1,1) | up | Mueve derecha |
| `x` | (1,1) | DOWN | Baja lapiz, pinta (1,1) |
| `d` | (2,1) | down | Mueve, pinta (2,1) |
| `d` | (3,1) | down | Mueve, pinta (3,1) |
| `s` | (3,2) | down | Mueve, pinta (3,2) |
| `s` | (3,3) | down | Mueve, pinta (3,3) |
| `a` | (2,3) | down | Mueve, pinta (2,3) |
| `a` | (1,3) | down | Mueve, pinta (1,3) |
| `w` | (1,2) | down | Mueve, pinta (1,2) |
| `w` | (1,1) | down | Mueve, pinta (1,1) — ya estaba pintada |

Resultado: cuadrado hueco en (1,1)-(3,3). Sin iteraciones (0), se imprime tal cual.

---

## Cosas que pueden preguntarte en el examen

| Pregunta | Respuesta |
|----------|-----------|
| Por que `calloc` y no `malloc`? | `calloc` inicializa a cero. Necesitamos que todas las celdas empiecen muertas (0). |
| Por que dos tableros? | Para no contaminar la generacion actual mientras calculas la siguiente. Sin esto, las celdas ya actualizadas afectarian el conteo de vecinos. |
| Que pasa cuando el lapiz intenta salir del tablero? | No se mueve. Los limites son: `x >= 0`, `x < width`, `y >= 0`, `y < height`. |
| Que hace `x` exactamente? | Alterna el estado del lapiz. La primera vez lo baja, la segunda lo sube, etc. |
| Las celdas fuera del tablero estan vivas o muertas? | Muertas. El enunciado lo dice: "each cell outside of the array will be considered dead". |
| Por que el swap de punteros y no copia? | Es O(1) en vez de O(width*height). Efficient y simple. |

---

## Resumen rapido para memorizar

1. **Lapiz** — empieza arriba en (0,0); `x` lo alterna; pinta DESPUES de moverse
2. **WASD** — w=arriba(y-1), a=izquierda(x-1), s=abajo(y+1), d=derecha(x+1)
3. **Tablero = array 1D** — acceso con `board[y * width + x]`
4. **Reglas Conway** — viva con 2-3 sobrevive, muerta con exactamente 3 nace, todo lo demas muere/muere
5. **Double buffering** — calcular nueva generacion en buffer separado, luego swap de punteros
6. **Bordes** — celdas fuera del tablero = muertas; lapiz no sale del tablero
7. **`calloc`** — inicializa a 0; `putchar('0')` para viva, `putchar(' ')` para muerta