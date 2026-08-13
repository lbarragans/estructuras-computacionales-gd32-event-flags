# 2. Compilacion y programacion

## Flujo recomendado en VS Code

Abra `Terminal > Run Task`. Las tareas estan numeradas en el orden de uso:

| Tarea | Funcion |
| --- | --- |
| `1. Verificar entorno GD32` | Comprueba las herramientas y rutas |
| `2. Configurar CMake (Debug)` | Genera Ninja en `build/debug` |
| `3. Compilar GD32 (Debug)` | Construye el firmware y sus artefactos |
| `4. Programar GD32 (Debug)` | Graba y verifica el ELF mediante OpenOCD |
| `5. Compilar y programar GD32` | Ejecuta automaticamente los pasos 2, 3 y 4 |
| `6. Preparar depuracion` | Crea el `launch.json` local |

Para el uso habitual basta ejecutar `5. Compilar y programar GD32`.

## Que hace CMake

CMake recopila `Src/main.c`, `Src/events.c`, el arranque RISC-V y los drivers
del SDK. Ninja ejecuta el compilador cruzado y el enlazador. Al finalizar se
crean en `build/debug`:

- `GD32VW55x.elf`: ejecutable usado para programar y depurar;
- `GD32VW55x.hex` y `GD32VW55x.bin`: imagenes alternativas de firmware;
- `GD32VW55x.map`: mapa de memoria;
- `GD32VW55x.lst`: codigo ensamblador desensamblado.

## Resultado de la programacion

El resultado correcto contiene:

```text
** Programming Finished **
** Verified OK **
** Resetting Target **
```

El LED debe permanecer cinco segundos lento y cinco segundos rapido.

Los scripts de `tools/` son auxiliares internos de las tareas en Windows. El
estudiante no necesita ejecutarlos directamente.
