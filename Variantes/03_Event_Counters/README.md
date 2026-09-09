# 03 - Contadores por evento

## Lenguaje

C.

## Diferencia fundamental

Bitmask:

```text
ocurrio
```

Counter:

```text
ocurrio N veces
```

## Ventaja

No colapsa multiples ocurrencias del mismo tipo.

## Advertencia

Los tres exchanges son atomicos individualmente, pero el conjunto no representa
necesariamente un unico instante global. Si se necesita un snapshot
transaccional de todos los contadores, hace falta otra estrategia.

## Estado

**Fuente lista.**
