# Vect2 — Explicacion para el Examen

## Enunciado

Crear una clase `vect2` que represente un vector matematico de dimension 2 con enteros. Debe soportar:
- **Suma** (`+`, `+=`) y **Resta** (`-`, `-=`)
- **Multiplicacion por escalar** (`vect2 * 2` y `2 * vect2`)
- **Acceso por indice** (`v[0]`, `v[1]`)
- **Impresion** con `<<`: formato `{x, y}`
- **Pre/Post incremento/decremento** (`++`, `--`)
- **Negacion unaria** (`-vect2`)
- **Igualdad y desigualdad** (`==`, `!=`)

Forma canonica ortodoxa obligatoria.

---

## Conceptos Clave a Recordar

### 1. Estructura de la clase — dos ints privados

```cpp
class vect2
{
    private:
        int _x;
        int _y;
```

Simple y directo: dos coordenadas enteras. No hay array interno, no hay `std::vector`.

### 2. `operator[]` — dos versiones: const y no-const

Este es uno de los puntos mas importantes. Necesitas DOS sobrecargas:

```cpp
int&   operator[](int index);       // Para asignacion: v[1] = 12;
int     operator[](int index) const; // Para lectura en objetos const: v3[1]
```

> **Regla de oro**: Si un objeto es `const`, solo puede llamar metodos `const`. Sin la version const, `v3[1]` no compilaria cuando `v3` es `const vect2`.

Implementacion:

```cpp
int& vect2::operator[](int index)
{
    if (index == 0)
        return (_x);
    else
        return (_y);
}

int vect2::operator[](int index) const
{
    if (index == 0)
        return (_x);
    else
        return (_y);
}
```

> **Nota**: La version no-const devuelve **referencia** para permitir asignacion: `v[1] = 12`.
> La version const devuelve por **valor** porque no puedes modificar un objeto const.

### 3. Multiplicacion por escalar — dos funciones

El enunciado requiere `(vect2(2,2) * 2 == vect2(4,4))`. Pero el main tambien usa `3 * v2`:

```cpp
v2 = 3 * v2;  // El escalar esta a la IZQUIERDA
```

C++ no puede interpretar `3 * vect2` con un miembro de la clase. Necesitas una **funcion no-miembro** (friend o libre):

```cpp
// Miembro: vect2 * escalar
vect2 vect2::operator*(int to_mult) const
{
    vect2 tmp(*this);
    tmp._x = _x * to_mult;
    tmp._y = _y * to_mult;
    return (tmp);
}

// No-miembro: escalar * vect2
vect2 operator*(int num, const vect2& to_mult)
{
    vect2 temp(to_mult);
    temp *= num;
    return temp;
}
```

> **Regla de oro**: Cuando el operador requiere que el lado izquierdo NO sea un objeto de la clase, se define como funcion libre (no-miembro).

### 4. Pre-incremento vs Post-incremento

Patron clasico de C++ que **siempre aparece en examenes**:

```cpp
// Pre-incremento: ++v → modifica Y devuelve referencia
vect2& vect2::operator++()
{
    _x++;
    _y++;
    return (*this);
}

// Post-incremento: v++ → devuelve copia del valor ANTES de modificar
vect2 vect2::operator++(int)  // el "int" es un dummy para distinguir
{
    vect2 tmp(*this);  // copia el estado actual
    _x++;
    _y++;
    return (tmp);      // devuelve la copia vieja
}
```

> **Regla**: El `int` en `operator++(int)` es un parametro dummy que C++ usa para distinguir post-incremento de pre-incremento. No se usa, es solo firma.

### 5. Negacion unaria (`-vect2`)

```cpp
vect2 vect2::operator-() const
{
    vect2 tmp(*this);
    tmp._x = -_x;
    tmp._y = -_y;
    return (tmp);
}
```

Devuelve un nuevo vector con coordenadas negadas. No modifica el original.

### 6. Operador `<<` — funcion no-miembro

El formato es `{x, y}` (con coma y espacio):

```cpp
std::ostream& operator<<(std::ostream& os, const vect2& toPrint)
{
    os << "{" << toPrint[0] << ", " << toPrint[1] << "}";
    return os;
}
```

> **Nota**: Usa `toPrint[0]` y `toPrint[1]`, no `toPrint._x`. Como `toPrint` es const, se llama a la version const de `operator[]`.

### 7. Los operadores compuestos devuelven referencia

```cpp
vect2& vect2::operator+=(const vect2& to_add)
{
    _x += to_add._x;
    _y += to_add._y;
    return (*this);
}
```

> **Regla**: `+=`, `-=`, `*=` devuelven `*this` por referencia. Los operadores simples `+`, `-`, `*` devuelven por valor (un nuevo objeto).

---

## Lineas tricky del main

```cpp
v2 += v2 += v3;  // Asociatividad derecha-a-izquierda
// v2 += (v2 += v3)
// Primero: v2 = v2 + v3 (= {1,2} + {1,2} = {2,4})
// Despues: v2 += {2,4} → v2 = {2,4} + {2,4} = ... 
// NOTA: v2 ya fue modificada por el += interior!
```

> **Cuidado**: `+=` se evalua de derecha a izquierda. El resultado del `+=` interior modifica `v2` ANTES de que se evalue el exterior.

```cpp
v1 = v1 - v1 + v1;  // {0, 0} + v1 = v1
```

> La resta tiene la misma precedencia que la suma, se evalua izquierda a derecha: `(v1 - v1) + v1 = {0,0} + v1 = v1`.

---

## Cosas que pueden preguntarte en el examen

| Pregunta | Respuesta |
|----------|-----------|
| Por que dos versiones de `operator[]`? | La version const permite acceso en objetos const. La no-const devuelve referencia para asignar. |
| Por que `operator*(int, vect2)` es funcion libre? | Porque el lado izquierdo es `int`, no `vect2`. Los miembros de clase solo funcionan cuando el lado izquierdo es un objeto de la clase. |
| Que significa el `int` en `operator++(int)`? | Es un parametro dummy. C++ lo usa para distinguir post-incremento de pre-incremento en la firma. |
| Devuelve `operator+` referencia o valor? | Valor. Siempre devuelve un objeto nuevo. Devolver referencia a local seria undefined behavior. |
| Que formato tiene `operator<<`? | `{x, y}` — con llaves, coma y espacio. Exactamente como lo dice el enunciado. |

---

## Resumen rapido para memorizar

1. **Dos `operator[]`** — const (devuelve valor) y no-const (devuelve referencia para asignar)
2. **`operator*` escalar** — miembro para `vect * int`, funcion libre para `int * vect`
3. **Pre vs Post `++`** — pre devuelve `*this&`, post devuelve copia temporal; el `int` es dummy
4. **Negacion unaria** — `operator-()` sin parametros, devuelve copia negada
5. **`<<`** — funcion libre, formato `"{x, y}"`
6. **OCAF** — default, copy, assign, destructor
7. **`+=` devuelve referencia**, `+` devuelve valor — siempre