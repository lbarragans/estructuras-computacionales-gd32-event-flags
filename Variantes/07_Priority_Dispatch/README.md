# 07 - Despacho con prioridad de software

## Lenguaje

C.

## Caso

Para:

```text
snapshot = 0x07
```

el orden elegido es:

```text
MODE_CHANGE
ONE_SECOND
LED_TICK
```

## Importante

Esta es prioridad de **despacho de eventos**, no prioridad de interrupcion ECLIC.

Son conceptos diferentes.

## Estado

**Fuente lista.**
