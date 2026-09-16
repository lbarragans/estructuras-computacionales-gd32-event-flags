# Ejercicio 05 - Interrupciones y banderas de eventos

**Curso:** Estructuras Computacionales

**Autora:** Laura Daniela Barragan Silva

**Plataforma:** GD32VW553HMQ6/HMQ7, RISC-V RV32

## Eventos

| Evento | Bit | Periodo |
|---|---:|---:|
| LED tick | `0x01` | 250 ms |
| Un segundo | `0x02` | 1000 ms |
| Cambio de modo | `0x04` | 5000 ms |

## Tres caminos

| Camino | Archivos | Concepto |
|---|---|---|
| Referencia | `Src/main.c`, `Src/events.c`, `Inc/events.h` | bitmask atomico con intrinsecos C |
| RISC-V puro | `Ensamblador_RISCV_Puro/main.S` | `amoor.w`, `amoswap.w`, ISR y despacho sin C |
| FreeRTOS puro | `FreeRTOS_Puro/main.c` | tareas y Event Groups |

La referencia original permanece como compilacion principal. Las diez
variantes antiguas fueron retiradas.

## Estructura

```text
05_Interrupt_Event_Flags/
├── Src/
├── Inc/
├── Ensamblador_RISCV_Puro/
├── FreeRTOS_Puro/
├── Doc/
├── CMakeLists.txt
└── README.md
```

## Estado

| Implementacion | Estado |
|---|---|
| Referencia C | funcional |
| Assembly puro | fuente lista; integracion y placa pendientes |
| FreeRTOS | fuente lista; kernel/port e integracion pendientes |
