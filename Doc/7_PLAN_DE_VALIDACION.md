# 7. Plan de validacion

## Comportamiento comun

- LED tick cada 250 ms;
- evento de segundo cada 1000 ms;
- cambio lento/rapido cada 5000 ms;
- prioridad de despacho: modo, segundo, LED;
- trabajo de fondo activo mientras no hay eventos.

## Referencia

Compilar con `build-debug` y observar flags, snapshots, publicaciones,
segundos, cambios de modo y toggles.

## Assembly

Integrar `main.S` sin C de aplicacion. Verificar extension A, operaciones AMO,
ISR de 1 ms, coalescencia, claim atomico y contadores globales.

## FreeRTOS

Integrar kernel, port, heap, tick y configuracion. Verificar set/wait/clear de
bits, prioridades y bloqueo eficiente del consumidor.

Ninguna ruta se marca validada sin compilacion y prueba fisica.
