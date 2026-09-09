# 01 - Bitmask atomico actual

## Lenguaje

C.

## Publicacion

```c
__atomic_fetch_or(&g_event_flags, mask, __ATOMIC_RELEASE);
```

## Claim

```c
__atomic_exchange_n(&g_event_flags, 0U, __ATOMIC_ACQ_REL);
```

## Semantica

Cada bit significa:

```text
este tipo de evento esta pendiente
```

No representa cantidad.

## Estado

**Base actual.**
