# Exercise 05 - Interrupciones y eventos en GD32VW553: flags, contadores, colas y Event Groups

**Curso:** Estructuras Computacionales
**Autora:** Laura Daniela Barragan Silva
**Plataforma:** GD32VW553HMQ6/HMQ7
**Arquitectura:** Nuclei RISC-V RV32
**Entorno:** Visual Studio Code, CMake, Ninja, Nuclei RISC-V GCC y OpenOCD

## 1. Proposito

La implementacion principal del repositorio se conserva intacta:

```text
Src/events.c
Src/main.c
Inc/events.h
```

La solucion actual separa dos contextos:

```text
SysTimer ISR
   ↓ publica bits
g_event_flags
   ↓ claim atomico
main
   ↓
dispatch
```

Los eventos actuales son:

| Evento | Mascara | Periodo |
|---|---:|---:|
| `EVENT_LED_TICK` | `0x01` | 250 ms |
| `EVENT_ONE_SECOND` | `0x02` | 1000 ms |
| `EVENT_MODE_CHANGE` | `0x04` | 5000 ms |

La ISR usa `__atomic_fetch_or()` y `main()` usa
`__atomic_exchange_n()` para reclamar y limpiar un snapshot de forma atomica.

La pregunta central de esta fase es:

> ¿Cuando conviene representar eventos como bits, contadores, registros de
> eventos, una cola, instrucciones atomicas RISC-V o primitivas de un RTOS?

## 2. Menu de implementaciones

| # | Variante | Lenguaje / tecnologia | Idea principal | Estado |
|---:|---|---|---|---|
| 01 | Bitmask atomico actual | C | OR atomico + exchange atomico | Base actual |
| 02 | Despacho mediante tabla de handlers | C | desacoplar bits de funciones | Fuente lista |
| 03 | Contadores por evento | C | conservar multiplicidad | Fuente lista |
| 04 | Ring buffer de eventos | C | conservar orden y repeticiones | Fuente lista |
| 05 | Eventos con timestamp y payload | C | transportar contexto | Fuente lista |
| 06 | Atomicos RISC-V con AMO | C + Assembly | `amoor.w` y `amoswap.w` | Fuente lista |
| 07 | Despacho con prioridad | C | orden determinista de atencion | Fuente lista |
| 08 | Laboratorio de coalescencia | C | medir cuando los bits colapsan eventos | Fuente lista |
| 09 | FreeRTOS Event Groups | C + FreeRTOS | equivalente RTOS natural del bitmask | Integracion pendiente |
| 10 | Flags vs counters vs queue vs notification | Documentacion | seleccion arquitectonica | Analisis |

Las fuentes educativas estan en [`Variantes/`](Variantes/README.md).

> El `CMakeLists.txt` principal no se modifica en esta fase. La implementacion
> original continua siendo la que compila por defecto.

## 3. Por que el bitmask atomico actual funciona

Publicar un evento significa:

```text
flags = flags OR mask
```

El problema es que la ISR puede modificar `flags` al mismo tiempo que `main`
intenta leerlo y limpiarlo.

La solucion actual evita una secuencia vulnerable como:

```text
snapshot = flags
flags = 0
```

y usa un intercambio atomico:

```c
snapshot = __atomic_exchange_n(&g_event_flags, 0U, __ATOMIC_ACQ_REL);
```

De esta forma, un evento que llegue simultaneamente queda en el valor antiguo
reclamado o en el nuevo valor pendiente, pero no se pierde por una ventana
lectura-modificacion-escritura.

## 4. Propiedad y limite de las banderas

Una bandera indica:

```text
ocurrio al menos una vez
```

No necesariamente:

```text
ocurrio exactamente N veces
```

Si `EVENT_LED_TICK` ocurre tres veces antes de que `main()` reclame los bits:

```text
bit 0 OR bit 0 OR bit 0 = bit 0
```

El consumidor observa una sola bandera.

Esto se llama **coalescencia** y no siempre es un error: depende de la semantica
que necesita la aplicacion.

## 5. Contadores por evento

Si la aplicacion necesita saber cuantas veces ocurrio un evento, una alternativa
es:

```text
led_tick_count++
second_count++
mode_change_count++
```

El consumidor reclama contadores en lugar de bits.

Ventaja:

- conserva multiplicidad.

Costo:

- mas memoria;
- mas operaciones;
- manejo de overflow;
- reclamar varios contadores de manera consistente puede requerir una estrategia
  atomica adicional.

## 6. Ring buffer

Cuando importa tanto la multiplicidad como el orden:

```text
A
B
A
C
```

un bitmask no puede representar esa secuencia.

Una cola circular almacena eventos individualmente:

```text
producer/ISR -> ring buffer -> consumer/main
```

Esto introduce nuevas preguntas:

- ¿que pasa si la cola se llena?
- ¿drop newest o drop oldest?
- ¿SPSC o multiples productores?
- ¿que operaciones deben ser atomicas?

## 7. Eventos con datos

Un sistema real puede necesitar mas que un tipo de evento.

Ejemplo:

```c
typedef struct {
    event_type_t type;
    uint32_t timestamp_ms;
    uint32_t value;
} event_t;
```

Ahora el evento puede significar:

```text
ADC_READY, timestamp=12345, value=2870
```

La variante 05 introduce esta idea.

## 8. Atomicos RISC-V

El nucleo soporta la extension atomica A.

La variante 06 implementa explicitamente:

```text
publish -> amoor.w
claim   -> amoswap.w
```

Esto conecta:

```text
intrinseco C
   ↓
modelo atomico
   ↓
instruccion RISC-V
```

Assembly se usa aqui porque aporta aprendizaje directo sobre el mecanismo
atomico; no se reescribe artificialmente el resto de la aplicacion.

## 9. Prioridad de eventos

Un snapshot puede contener:

```text
0x07
```

es decir, varios eventos simultaneos.

El software debe decidir el orden de despacho.

La variante 07 hace explicita una prioridad:

```text
MODE_CHANGE
ONE_SECOND
LED_TICK
```

y permite discutir si esa prioridad pertenece al mecanismo de transporte o a la
logica de aplicacion.

## 10. Coalescencia como experimento

La variante 08 instrumenta:

```text
eventos publicados
eventos reclamados
snapshots
coalescencias estimadas
```

La meta es observar que "un bit pendiente" y "una cuenta de ocurrencias" son
abstracciones diferentes.

## 11. FreeRTOS Event Groups

FreeRTOS ofrece **Event Groups**, una abstraccion muy cercana a este ejercicio:

```text
set bits
wait bits
clear bits
```

Es una evolucion natural del bitmask manual cuando el sistema ya utiliza RTOS.

No obstante, FreeRTOS no es un lenguaje y esta variante permanece como
**integracion pendiente** hasta enlazar kernel, port RISC-V, heap,
`FreeRTOSConfig.h` y tick.

## 12. Que estructura elegir

Regla orientativa:

```text
solo importa "ocurrio"             -> flags
importa "cuantas veces"            -> counters
importa orden/repeticion/payload   -> queue
un solo receptor RTOS              -> task notification puede bastar
varios bits de condicion RTOS      -> Event Group
```

No existe una unica respuesta correcta para todos los sistemas.

## 13. Lenguajes

Las implementaciones contienen realmente:

```text
C
Assembly RISC-V
```

Por eso ambos pueden aparecer en GitHub.

No deben alterar la barra:

```text
PowerShell
CMake
JSON
VS Code
Markdown
```

## 14. Validacion

Cada variante debe demostrar que:

- la ISR no ejecuta trabajo pesado de aplicacion;
- no existe una carrera obvia entre productor y consumidor;
- se documenta si los eventos pueden coalescer;
- se documenta que ocurre en overflow/cola llena;
- el LED conserva el comportamiento esperado cuando corresponda.

Ver [`Doc/7_PLAN_DE_VALIDACION.md`](Doc/7_PLAN_DE_VALIDACION.md).
