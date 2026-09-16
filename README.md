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
| Referencia C | validada en placa |
| Assembly puro | validada en placa |
| FreeRTOS | validada en placa con el MSDK V1.0.3g |

## Ejecutar las variantes

Referencia original por JTAG/OpenOCD:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\\tools\\build_variant.ps1 -Variant original -Flash
```

Assembly puro por JTAG/OpenOCD:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\\tools\\build_variant.ps1 -Variant assembly -Flash
```

En VS Code abra **Terminal > Run Task** y elija una de estas tareas:

- `Build + Flash Original`
- `Build + Flash Assembly`
- `Build + Flash FreeRTOS`

Las tres producen el mismo patron observable: el LED cambia entre modo lento
(conmutacion cada 500 ms) y modo rapido (cada 250 ms). El cambio ocurre cada
5 segundos. La bandera de un segundo se procesa internamente y puede observarse
en depuracion.

FreeRTOS usa el port oficial del SDK WiFi V1.0.3g. El script de VS Code copia
automaticamente `main.c` y `app_cfg.h` hacia `MSDK/app`, compila `image-all.bin`
y lo graba desde VS Code.

## Guía central de ejecución

Consulte la [guía central GD32VW553](https://github.com/lbarragans/gd32vw553-vscode-cmake-guide)
antes de compilar o integrar las variantes Assembly y FreeRTOS.
