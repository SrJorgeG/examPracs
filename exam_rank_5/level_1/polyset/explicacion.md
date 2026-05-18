# Polyset — Explicacion para el Examen

## Enunciado

Se te dan las clases base `bag`, `searchable_bag`, `array_bag` y `tree_bag`. Hay que implementar:

1. **`searchable_array_bag`** — hereda de `searchable_bag` y `array_bag`, implementa `has(int)`
2. **`searchable_tree_bag`** — hereda de `searchable_bag` y `tree_bag`, implementa `has(int)`
3. **`set`** — envuelve un `searchable_bag` y lo convierte en un conjunto (sin duplicados)

Todas las clases deben estar en **forma canonica ortodoxa**. No olvidar los `const`.

---

## Conceptos Clave a Recordar

### 1. Herencia Diamante y `virtual public`

Este es el punto mas importante del ejercicio. La jerarquia de clases forma un diamante:

```
            bag
           /    \
  searchable_bag   array_bag
           \    /
   searchable_array_bag
```

Si `searchable_bag` y `array_bag` heredan de `bag` de forma normal, `searchable_array_bag` tendria **DOS copias** de `bag` → **ambiguedad**. La solucion: **herencia virtual**.

```cpp
class searchable_bag : virtual public bag { ... };
class array_bag       : virtual public bag { ... };
class tree_bag        : virtual public bag { ... };
```

> Si ves `virtual public` en la herencia, es para resolver el **diamond problem**. Sin esto, el compilador se queja de ambiguedad al llamar metodos de `bag`.

### 2. La clase `searchable_array_bag`

Hereda de BOTH `searchable_bag` y `array_bag`:

```cpp
class searchable_array_bag : public searchable_bag, public array_bag
```

Implementa `has(int)` haciendo una busqueda lineal en el array:

```cpp
bool searchable_array_bag::has(int value) const
{
    for (int i = 0; i < size; i++)  // size es protected en array_bag
    {
        if (data[i] == value)        // data es protected en array_bag
            return (true);
    }
    return (false);
}
```

> **Nota clave**: `size` y `data` son `protected` en `array_bag`, por lo que las clases derivadas pueden accederlos directamente. Si fueran `private`, necesitarias getters.

#### Constructor de copia y operador de asignacion

Como hay herencia multiple con virtual, la inicializacion del constructor de copia DEBE llamar al constructor de copia de `array_bag`:

```cpp
searchable_array_bag::searchable_array_bag(const searchable_array_bag& other) : array_bag(other)
{
}
```

> El `searchable_bag` no tiene miembros de datos, asi que no necesita inicializacion explicita. Pero `array_bag` si tiene `data` y `size`.

Para el operador de asignacion, se delega al padre:

```cpp
searchable_array_bag& searchable_array_bag::operator=(const searchable_array_bag& other)
{
    if (this != &other)
        array_bag::operator=(other);
    return (*this);
}
```

### 3. La clase `searchable_tree_bag`

Similar a `searchable_array_bag` pero hereda de `tree_bag`. La busqueda utiliza la propiedad del BST (arbol binario de busqueda):

```cpp
bool searchable_tree_bag::search(node* node, int value) const
{
    if (!node)
        return (false);
    if (node->value == value)
        return (true);
    else if (value < node->value)
        return (search(node->l, value));  // buscar a la izquierda
    else
        return (search(node->r, value));  // buscar a la derecha
}

bool searchable_tree_bag::has(int value) const
{
    return (search(this->tree, value));
}
```

> **Nota clave**: `tree` es `protected` en `tree_bag`, y `node` es un struct protegido dentro de `tree_bag`. Como `searchable_tree_bag` hereda de `tree_bag`, puede acceder a ambos.

> **OJO**: El metodo privado `search` recibe un `node*`. Como `node` es un tipo protegido dentro de `tree_bag`, la firma es `bool search(node* node, int value) const` donde `node` se refiere a `tree_bag::node`.

### 4. La clase `set` — Patron Wrapper/Adapter

`set` NO hereda de `searchable_bag`. **Envuelve** (wraps) una referencia a `searchable_bag`:

```cpp
class set
{
    private:
        searchable_bag& _bag;   // Referencia, no puntero!
        set();                    // constructor por defecto privado
        set(const set& other);   // constructor de copia privado
        set& operator=(const set& other);  // asignacion privada
    public:
        set(searchable_bag& s_bag);
        // ...
};
```

> **Cosas criticas del set**:
> 1. Usa una **referencia** (`searchable_bag&`), no un puntero, no una copia. Esto significa que el `searchable_bag` debe vivir fuera del set.
> 2. Los constructores por defecto, copia y asignacion estan **privados** → el set NO se puede copiar ni crear sin un bag existente. Esto es porque la referencia no se puede reasignar.
> 3. `insert(int)` verifica duplicados con `has()` antes de insertar. Eso es lo que lo convierte de bag a set.

```cpp
void set::insert(int value)
{
    if (!(has(value)))      // Solo inserta si NO existe ya
        _bag.insert(value);
}
```

> **TRAMPA**: `insert(int*, int)` del set NO verifica duplicados! Itera y llama a `_bag.insert` directamente. Esto puede ser un bug o un diseno intencional del examen.

### 5. Polimorfismo en el main

El main demuestra polimorfismo a traves de punteros a la clase base:

```cpp
searchable_bag *t = new searchable_tree_bag;
searchable_bag *a = new searchable_array_bag;
```

Un puntero a `searchable_bag` apunta a cualquier implementacion concreta. Las llamadas a `insert()`, `has()`, `print()` se resuelven dinamicamente en tiempo de ejecucion.

---

## Jerarquia de Clases Completa

```
                    bag (virtual)
                   /    |        \
        searchable_bag  array_bag  tree_bag
              \          |            /
        searchable_    searchable_   searchable_
         array_bag      _array_bag   tree_bag
                                  \
                                   set (wrapper)
```

---

## Cosas que pueden preguntarte en el examen

| Pregunta | Respuesta |
|----------|-----------|
| Por que `virtual public` en la herencia? | Para evitar el diamond problem: sin virtual, la clase derivada tendria dos copias de `bag`. |
| Puede `set` copiarse? | No, el constructor de copia y el operador de asignacion estan privados. Tiene una referencia que no se puede reasignar. |
| Que diferencia hay entre bag y set? | Un bag permite duplicados. Un set NO. La clase `set` lo logra verificando `has()` antes de `insert()`. |
| Por que `searchable_array_bag` puede acceder a `data` y `size`? | Porque son `protected` en `array_bag`, y las clases derivadas tienen acceso a miembros protegidos. |
| Por que `set` usa referencia y no herencia? | Porque `set` es un **adaptador**: no es un bag, *contiene* un bag. Esto es composicion, no herencia. |
| Que pasa si se intenta copiar un `set`? | Error de compilacion: el constructor de copia y `operator=` estan inaccesibles (privados). |

---

## Resumen rapido para memorizar

1. **Herencia virtual** (`virtual public`) → soluciona el diamond problem
2. **`searchable_array_bag`** → hereda de ambos, busca en array O(n), delega constructores a `array_bag`
3. **`searchable_tree_bag`** → hereda de ambos, busca en BST O(log n), usa metodo privado recursivo `search`
4. **`set`** → wrapper con referencia a `searchable_bag`, no se puede copiar, verifica duplicados con `has()`
5. **Miembros `protected`** → accesibles desde clases derivadas, necesario para `has()`
6. **Polimorfismo** → punteros a `searchable_bag` pueden apuntar a cualquiera de las implementaciones