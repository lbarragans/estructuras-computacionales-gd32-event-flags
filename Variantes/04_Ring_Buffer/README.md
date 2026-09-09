# 04 - Ring buffer de eventos

## Lenguaje

C.

## Modelo

Single Producer / Single Consumer:

```text
ISR  -> producer
main -> consumer
```

## Conserva

- orden;
- multiplicidad.

## Politica de cola llena

Esta variante rechaza el evento nuevo e incrementa `dropped`.

La ISR nunca bloquea.

## Estado

**Fuente lista.**
