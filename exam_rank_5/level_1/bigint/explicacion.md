# BigInt — Explicacion para el Examen

## Enunciado

Crear una clase `bigint` que almacene un entero sin signo de precision arbitraria. Debe soportar:
- **Suma** (`+`, `+=`)
- **Comparaciones** (`==`, `!=`, `<`, `>`, `<=`, `>=`)
- **Digit shift** (`<<`, `>>`) — como bitshift pero en base 10: `42 << 3 == 42000`, `1337 >> 2 == 13`
- **Impresion** con `<<` sin ceros a la izquierda
- **Incremento** (`++` pre y post)

---

## Conceptos Clave a Recordar

### 1. Representacion interna: `std::string`

Lo mas importante: **el numero se almacena como string en base 10**. Cada caracter del string es un digito. Esto permite numeros de precision arbitraria porque un string no tiene el limite de `SIZE_MAX`.

```
bigint(42) → _str = "42"
bigint(0)  → _str = "0"
```

### 2. Suma con acarreo (carry)

La suma funciona como la suma manual que aprendiste en primaria: vas de derecha a izquierda, sumas digito a digito, y propagas el acarreo.

```cpp
bigint& bigint::operator+=(const bigint& other)
{
    int i = _str.size() - 1;
    int j = other._str.size() - 1;
    int carry = 0;
    std::string result;
    
    while (i >= 0 || j >= 0 || carry)
    {
        int d1 = (i >= 0) ? _str[i--] - '0' : 0;
        int d2 = (j >= 0) ? other._str[j--] - '0' : 0;
        int sum = d1 + d2 + carry;
        result += (sum % 10) + '0';  // digito resultado
        carry = sum / 10;            // acarreo
    }
    std::reverse(result.begin(), result.end());
    _str = result;
    return (*this);
}
```

> **Nota clave**: El resultado se construye al reves porque vamos de derecha a izquierda. Despues hay que hacer `reverse`.

### 3. Digit Shift — La trampa del examen

El **digit shift** NO es un bitshift. Es multiplicar/dividir por potencias de 10:

- `<< n` → agregar `n` ceros al final (equivalente a multiplicar por 10^n)
- `>> n` → eliminar `n` digitos del final (equivalente a dividir entre 10^n)

```cpp
// Left shift: "42" << 3 → "42000" (agrega 3 ceros)
bigint& bigint::operator<<=(unsigned int shift)
{
    if (_str != "0")
        _str.append(shift, '0');
    return (*this);
}

// Right shift: "1337" >> 2 → "13" (borra los ultimos 2 digitos)
bigint& bigint::operator>>=(const bigint& obj)
{
    unsigned long long shift = str_to_number(obj._str);
    if (shift >= _str.size())
        _str = "0";
    else
        _str.erase(_str.size() - shift, shift);
    return (*this);
}
```

> **Caso especial**: Si el shift derecho es >= al tamaño del numero, el resultado es `"0"`.

### 4. Comparacion: primero longitud, luego lexicografico

Como los strings estan en base 10 sin ceros a la izquierda:
- Un numero mas largo SIEMPRE es mayor.
- Si tienen la misma longitud, comparacion lexicografica directa funciona correctamente.

```cpp
bool bigint::operator<(const bigint& other) const
{
    if (_str.size() != other._str.size())
        return (_str.size() < other._str.size());
    return (_str < other._str);
}
```

> **Nunca compares strings de diferente longitud lexicograficamente!** `"9" < "10"` seria falso si comparas como strings directamente.

### 5. Validacion: eliminar ceros a la izquierda

Despues de operaciones que puedan generar ceros a la izquierda, se llama `validate()`:

```cpp
void bigint::validate()
{
    if (_str.empty() || _str == "0")
        return ;
    size_t i = 0;
    while (i < _str.size() && _str[i] == '0')
        i++;
    if (i > 0)
        _str.erase(0, i);
}
```

### 6. Pre/Post incremento

```cpp
// Pre-incremento: devuelve la referencia al objeto ya incrementado
bigint& bigint::operator++()
{
    *this += bigint(1);
    return (*this);
}

// Post-incremento: devuelve copia del valor ANTES de incrementar
bigint bigint::operator++(int)
{
    bigint tmp(*this);  // copia antes
    *this += bigint(1);
    return (tmp);        // devuelve la copia
}
```

### 7. Forma Canonica Ortodoxa (OCAF)

La clase implementa el **Orthodox Canonical Form**:
- Constructor por defecto
- Constructor de copia
- Operador de asignacion
- Destructor

Todo esto es trivial porque `std::string` maneja su propia memoria.

---

## Cosas que pueden preguntarte en el examen

| Pregunta | Respuesta |
|----------|-----------|
| Por que usar `std::string` y no `int`? | Un `int` tiene limite de tamaño (overflow). Un string no tiene ese limite. |
| Por que `operator+=` devuelve referencia? | Para permitir encadenamiento: `a += b += c`. |
| Por que `operator+` devuelve por valor? | Porque crea un objeto nuevo. Devolver referencia seria colgante. |
| Que pasa si haces shift derecho mayor al numero? | Resultado es "0". Esta contemplado en el codigo. |
| Por que `operator<<` recibe `unsigned int` y `operator>>` recibe `bigint`? | El shift izquierdo es por un entero simple, el derecho se define como `>>=` con otro bigint segun el enunciado. |

---

## Resumen rapido para memorizar

1. **String como almacenamiento** — cada digito es un char, sin limite de precision
2. **Suma = carry manual** — de derecha a izquierda, modulo 10 y carry / 10
3. **`<<` agrega ceros, `>>` borra digitos del final** — no es bitshift, es base 10
4. **Comparar = longitud primero** — si longitudes difieren, ya sabes cual es mayor
5. **`validate()` limpia ceros a la izquierda** — despues de construir desde unsigned int
6. **OCAF** — constructor default, copy, assign, destructor