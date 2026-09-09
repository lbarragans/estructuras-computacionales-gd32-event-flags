# 6. Variantes del mecanismo de eventos

## Comparacion

| Mecanismo | Multiplicidad | Orden | Payload | Memoria | ISR corta |
|---|---:|---:|---:|---:|---:|
| bitmask | No | No | No | muy baja | Si |
| tabla de handlers | igual al bitmask | prioridad fija | No | baja | Si |
| counters | Si | No | No | baja/media | Si |
| ring buffer | Si | Si | tipo | media | Si |
| event records | Si | Si | Si | media/alta | Si |
| AMO Assembly | igual al bitmask | No | No | muy baja | Si |
| FreeRTOS Event Group | No por bit | No | bits | kernel | Si |

## Preguntas

1. ¿Que significa que una operacion sea atomica?
2. ¿Por que `volatile` no sustituye atomicidad?
3. ¿Que carrera existe en `snapshot = flags; flags = 0;`?
4. ¿Que garantiza un exchange atomico?
5. ¿Por que tres publicaciones del mismo bit pueden verse como una?
6. ¿Cuando esa coalescencia es aceptable?
7. ¿Cuando se necesitan contadores?
8. ¿Cuando se necesita una cola?
9. ¿Que informacion agrega un timestamp?
10. ¿Que diferencia hay entre prioridad de interrupcion y prioridad de despacho?
11. ¿Que hacen conceptualmente `amoor.w` y `amoswap.w`?
12. ¿Cuando un Event Group de FreeRTOS es mejor que una Queue?
