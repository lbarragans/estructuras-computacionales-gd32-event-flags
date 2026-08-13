# 4. Depuracion

## Crear configuracion

1. Abra `Terminal > Run Task`.
2. Ejecute una sola vez `6. Preparar depuracion`.
3. Abra `Run and Debug`.
4. Seleccione `Debug GD32VW553 - Cortex Debug`.
5. Pulse el boton verde. Si F5 no responde, use siempre ese boton.

La tarea crea `.vscode/launch.json` con las rutas del computador. Este archivo
es local y no debe publicarse.

## Breakpoints recomendados

En `Src/main.c`:

1. `g_last_event_snapshot = pending_events;`;
2. `handle_mode_change_event();`;
3. `g_seconds_handled++;`;
4. `led_toggle();`.

En `Src/events.c`:

1. `__atomic_fetch_or(...)` dentro de `publish_event()`;
2. `system_milliseconds++;` dentro de la ISR.

No deje detenido el breakpoint de la ISR: ocurre cada milisegundo.

## Watch

```text
g_event_flags
g_last_event_snapshot
g_isr_count
g_events_published
g_event_batches
g_seconds_handled
g_blink_mode
g_mode_changes
g_led_toggle_count
g_background_iterations
```

## Resultados esperados

- `g_isr_count` aumenta aproximadamente 1000 por segundo;
- `g_seconds_handled` aumenta una vez por segundo;
- `g_blink_mode` alterna `0 -> 1 -> 0` cada cinco segundos;
- `g_mode_changes` aumenta una vez por cambio;
- `g_last_event_snapshot` suele ser `1`, `3` y cada cinco segundos `7`;
- `g_background_iterations` crece en ambos modos.

## Comprobar la operacion atomica

Abra `build/debug/GD32VW55x.lst` y busque:

```text
amoswap
amoor
```

El nombre exacto puede variar con la optimizacion, pero debe existir una
operacion atomica o una secuencia equivalente suministrada por el compilador.
