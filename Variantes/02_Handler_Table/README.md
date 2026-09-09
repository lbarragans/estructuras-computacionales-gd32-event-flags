# 02 - Despacho mediante tabla de handlers

## Lenguaje

C.

## Idea

Se separa:

```text
bit de evento -> funcion manejadora
```

mediante una tabla.

## Ventajas

- elimina una cadena grande de `if`;
- facilita registrar nuevos handlers;
- el orden de la tabla hace explicita la prioridad de software.

## Estado

**Fuente lista.**
