# 7. Plan de validacion

## Comportamiento funcional base

El LED debe alternar:

```text
5 s modo lento: cambio cada 500 ms
5 s modo rapido: cambio cada 250 ms
repetir
```

Los eventos fuente continuan siendo:

```text
250 ms  -> LED_TICK
1000 ms -> ONE_SECOND
5000 ms -> MODE_CHANGE
```

## Caso simultaneo

En 5000 ms pueden coincidir:

```text
EVENT_LED_TICK
EVENT_ONE_SECOND
EVENT_MODE_CHANGE
```

Para bitmask:

```text
snapshot esperado = 0x07
```

## Carrera productor/consumidor

Probar que una publicacion que ocurre alrededor del momento del `claim` no se
pierde por una secuencia no atomica.

## Coalescencia

Generar el mismo evento varias veces antes del consumo.

Esperado:

### Flags

```text
N publicaciones -> 1 bit pendiente
```

### Counters

```text
N publicaciones -> contador aumenta N
```

### Queue

```text
N publicaciones -> N elementos si existe capacidad
```

## Cola llena

Toda variante de queue debe definir la politica:

- rechazar nuevo;
- sobrescribir antiguo;
- bloquear (solo si contexto lo permite).

Una ISR no debe bloquear esperando espacio.

## Assembly AMO

Observar:

- direccion en `a0`;
- mascara en `a1`;
- `amoor.w.aqrl`;
- `amoswap.w.aqrl`;
- valor anterior retornado por `claim`.

## FreeRTOS

Solo marcar validada despues de integrar:

- kernel;
- port RISC-V;
- `FreeRTOSConfig.h`;
- heap;
- scheduler;
- tick;
- Event Group;
- API ISR-safe cuando corresponda.

## Estados

- Base actual
- Fuente lista
- Analisis
- Integracion pendiente
- Validada por compilacion
- Validada en placa
